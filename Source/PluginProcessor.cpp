/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionPedalAudioProcessor::DistortionPedalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    drive = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter("Drive"));
    jassert(drive != nullptr);
    
    range = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter("Range"));
    jassert(range != nullptr);
    
    blend = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter("Blend"));
    jassert(blend != nullptr);
    
    volume = dynamic_cast<juce::AudioParameterFloat*>(state.getParameter("Volume"));
    jassert(volume != nullptr);
    
    
    
}

DistortionPedalAudioProcessor::~DistortionPedalAudioProcessor()
{
}

//==============================================================================
const juce::String DistortionPedalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionPedalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionPedalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionPedalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionPedalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionPedalAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionPedalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionPedalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionPedalAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionPedalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionPedalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistortionPedalAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionPedalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistortionPedalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++){
            float cleanSig = *channelData;
            *channelData *= drive->get()*range->get();
            *channelData = (((((2.0f/juce::MathConstants<float>::pi) * atan(*channelData))*blend->get())+(cleanSig * (1.0f / blend->get()))) / 2)*volume->get();
            channelData++;
        }
        
        

        // ..do something to the data...
    }
}

//==============================================================================
bool DistortionPedalAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionPedalAudioProcessor::createEditor()
{
//    return new DistortionPedalAudioProcessorEditor (*this);
return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void DistortionPedalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    state.state.writeToStream(mos);
    
    
    
}

void DistortionPedalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        state.replaceState(tree);
    }
}


juce::AudioProcessorValueTreeState::ParameterLayout DistortionPedalAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Drive",
                                                           "Drive",
                                                           juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f,1.f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Range",
                                                           "Range",
                                                           juce::NormalisableRange<float>( 0.1f, 20.0f, 0.1f,1.f), 0.1f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Blend",
                                                           "Blend",
                                                           juce::NormalisableRange<float>( 0.1f, 1.0f, 0.1f,1.f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Volume",
                                                           "Volume",
                                                           juce::NormalisableRange<float>( 0.0f, 2.0f, 0.1f,1.f), 0.0f));
    
    
    
    
    return layout;
    
//    AudioProcessorValueTreeState::ParameterLayout DistortionAudioProcessor::createParameterLayout(){
//        AudioProcessorValueTreeState::ParameterLayout layout;
//        layout.add(std::make_unique<AudioParameterFloat>(DRIVE_ID, DRIVE_NAME, 0.0f, 1.0f, 0.0f));
//        layout.add(std::make_unique<AudioParameterFloat>(RANGE_ID, RANGE_NAME, 0.1f, 20.0f, 0.1f));
//        layout.add(std::make_unique<AudioParameterFloat>(BLEND_ID, BLEND_NAME, 0.1f, 1.0f, 0.0f));
//        layout.add(std::make_unique<AudioParameterFloat>(VOLUME_ID, VOLUME_NAME, 0.0f, 2.0f, 0.0f));
//        return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPedalAudioProcessor();
}
