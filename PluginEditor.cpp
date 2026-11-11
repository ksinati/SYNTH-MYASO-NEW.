#include "PluginEditor.h"

SYNTH_MYASOAudioProcessorEditor::SYNTH_MYASOAudioProcessorEditor(
    SYNTH_MYASOAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(520, 330);

    title.setText("SYNTH.МЯСО", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(34.0f, juce::Font::bold)));
    title.setColour(juce::Label::textColourId, juce::Colours::white);
    title.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(title);

    subtitle.setText("ГЕНЕРАТОР ЗВУКА / BASEMENT AUDIO", juce::dontSendNotification);
    subtitle.setFont(juce::Font(juce::FontOptions(13.0f)));
    subtitle.setColour(juce::Label::textColourId, juce::Colour(0xffff5555));
    subtitle.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subtitle);

    volume.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 24);
    volume.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffd51f2a));
    volume.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    volume.setTextValueSuffix(" %");
    addAndMakeVisible(volume);

    volumeAttachment = std::make_unique<
        juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.parameters, "volume", volume);
}

void SYNTH_MYASOAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff100b0c));
    g.setColour(juce::Colour(0xff5b1018));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(12.0f), 14.0f);
    g.setColour(juce::Colour(0xff1a1012));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(17.0f), 11.0f);
    g.setColour(juce::Colour(0xffb51f2b));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(17.0f), 11.0f, 2.0f);

    g.setColour(juce::Colour(0xff6d171f));
    for (int y = 95; y < 310; y += 18)
        g.drawHorizontalLine(y, 28.0f, 492.0f);

    g.setColour(juce::Colour(0xffe13a45));
    g.setFont(juce::Font(juce::FontOptions(15.0f, juce::Font::bold)));
    g.drawFittedText("ГУЛ В КИШКАХ", 55, 265, 410, 24,
                     juce::Justification::centred, 1);
}

void SYNTH_MYASOAudioProcessorEditor::resized()
{
    title.setBounds(25, 25, 470, 48);
    subtitle.setBounds(25, 73, 470, 24);
    volume.setBounds(185, 110, 150, 145);
}
