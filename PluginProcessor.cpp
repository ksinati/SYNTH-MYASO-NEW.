#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
class BasicSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

class BasicVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<BasicSound*>(sound) != nullptr;
    }

    void startNote(int midiNote, float velocity, juce::SynthesiserSound*, int) override
    {
        level = velocity * 0.18f;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNote);
        phase = 0.0;
        tailOff = 0.0f;
    }

    void stopNote(float, bool allowTailOff) override
    {
        if (allowTailOff) tailOff = 0.99f;
        else clearCurrentNote();
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(juce::AudioBuffer<float>& output, int start, int count) override
    {
        if (frequency <= 0.0) return;
        const double increment = juce::MathConstants<double>::twoPi * frequency
                               / getSampleRate();

        while (count-- > 0)
        {
            float sample = static_cast<float>(std::sin(phase)) * level;
            phase += increment;
            if (phase >= juce::MathConstants<double>::twoPi)
                phase -= juce::MathConstants<double>::twoPi;

            if (tailOff > 0.0f)
            {
                sample *= tailOff;
                tailOff *= 0.9992f;
                if (tailOff < 0.005f)
                {
                    clearCurrentNote();
                    break;
                }
            }

            for (int channel = 0; channel < output.getNumChannels(); ++channel)
                output.addSample(channel, start, sample);
            ++start;
        }
    }

private:
    double phase = 0.0;
    double frequency = 0.0;
    float level = 0.0f;
    float tailOff = 0.0f;
};
}

SYNTH_MYASOAudioProcessor::SYNTH_MYASOAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    for (int i = 0; i < 8; ++i)
        synth.addVoice(new BasicVoice());
    synth.addSound(new BasicSound());
}

juce::AudioProcessorValueTreeState::ParameterLayout
SYNTH_MYASOAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back(std::make_unique<juce::AudioParameterFloat>(
        "volume", "ГРОМКОСТЬ", juce::NormalisableRange<float>(0.0f, 1.0f), 0.7f));
    return { p.begin(), p.end() };
}

void SYNTH_MYASOAudioProcessor::prepareToPlay(double sampleRate, int)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SYNTH_MYASOAudioProcessor::releaseResources() {}

void SYNTH_MYASOAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());

    const float volume = parameters.getRawParameterValue("volume")->load();
    buffer.applyGain(volume);
}

juce::AudioProcessorEditor* SYNTH_MYASOAudioProcessor::createEditor()
{
    return new SYNTH_MYASOAudioProcessorEditor(*this);
}

void SYNTH_MYASOAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, dest);
}

void SYNTH_MYASOAudioProcessor::setStateInformation(const void* data, int size)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SYNTH_MYASOAudioProcessor();
}
