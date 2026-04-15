/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class KillswitchAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    KillswitchAudioProcessor();
    ~KillswitchAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    // 物理建模参数
    bool isMuted = false;          // 当前开关状态
    bool lastMutedState = false;   // 用于检测边缘触发
    float dcBias = 0.0f;           // 模拟电荷释放的直流偏移
    juce::Random randomSource;     // 模拟断路时的底噪

    // 注意：这里的宏里面的名字必须和你 Projucer 里的项目名一致（首字母大写）
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KillswitchAudioProcessor)
};
