#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SYNTH_MYASOAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SYNTH_MYASOAudioProcessorEditor(SYNTH_MYASOAudioProcessor&);
    ~SYNTH_MYASOAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SYNTH_MYASOAudioProcessor& audioProcessor;
    juce::Slider volume;
    juce::Label title;
    juce::Label subtitle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SYNTH_MYASOAudioProcessorEditor)
};
