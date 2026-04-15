/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KillswitchAudioProcessor::KillswitchAudioProcessor()
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
    // 初始化变量，防止产生尖锐杂音
    isMuted = false;
    lastMutedState = false;
    dcBias = 0.0f;
}

KillswitchAudioProcessor::~KillswitchAudioProcessor()
{
}

//==============================================================================
const juce::String KillswitchAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KillswitchAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KillswitchAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KillswitchAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KillswitchAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KillswitchAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KillswitchAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KillswitchAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KillswitchAudioProcessor::getProgramName (int index)
{
    return {};
}

void KillswitchAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KillswitchAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KillswitchAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KillswitchAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KillswitchAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto numSamples = buffer.getNumSamples();

    // --- 1. MIDI 信号解析 ---
    // 我们遍历这一帧里所有的 MIDI 消息
    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();

        if (msg.isNoteOn())
            isMuted = true;   // 按下任意键，断路
        else if (msg.isNoteOff())
            isMuted = false;  // 松开按键，通路
    }

    // --- 2. 音频处理循环 ---
    for (int sample = 0; sample < numSamples; ++sample)
    {
        dcBias *= 0.98f; // 物理冲击衰减

        if (isMuted != lastMutedState)
        {
            dcBias = 0.07f; // 稍微调强一点物理冲击感
        }

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            float inputSample = channelData[sample];

            if (isMuted)
            {
                // 断路：输出直流位移 + 微弱底噪
                float noise = (randomSource.nextFloat() * 2.0f - 1.0f) * 0.0001f;
                channelData[sample] = dcBias + noise;
            }
            else
            {
                // 通路：信号通过 + 冲击
                channelData[sample] = inputSample + dcBias;
            }
        }
        lastMutedState = isMuted;
    }
}

//==============================================================================
bool KillswitchAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KillswitchAudioProcessor::createEditor()
{
    return new KillswitchAudioProcessorEditor (*this);
}

//==============================================================================
void KillswitchAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KillswitchAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KillswitchAudioProcessor();
}
