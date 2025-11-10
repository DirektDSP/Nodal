#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "moonbase_JUCEClient/moonbase_JUCEClient.h"
#include "BinaryData.h"
#include "Service/PresetManager.h"
#include "DSP/ChasmDSP.h"

#if (MSVC)
#include "ipps.h"
#endif

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    MOONBASE_DECLARE_LICENSING_NAMED(BinaryData, "DirektDSP", "plugintemplate", VERSION)

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    AudioProcessorValueTreeState& getApvts() { return apvts; }

    Service::PresetManager& getPresetManager() { return *presetManager; }

    juce::AudioProcessorValueTreeState apvts;


    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
        
        // Input/Output Gains (-24 to +24 dB, Linear, 5ms smoothing)
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{"INPUT_GAIN", 1}, "Input", 
            juce::NormalisableRange<float>(-48.0f, 24.0f, 0.1f), 0.0f));
            
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{"OUTPUT_GAIN", 1}, "Output", 
            juce::NormalisableRange<float>(-48.0f, 24.0f, 0.1f), 0.0f));
        
        // Mix (0 to 100%, Linear, 20ms smoothing)
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{"MIX", 1}, "Mix", 
            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
        
        // High Cut Logarithmic (20hz to 20khz, Linear, 20ms smoothing)
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{"HIGH_CUT", 1}, "High Cut",
            juce::NormalisableRange<float>(0.0f, 20000.0f, 0.1f, 0.2f), 20000.0f,
            juce::String(),
            juce::AudioProcessorParameter::genericParameter,
            [](float value, int) {
                if (value >= 20000.0f)
                    return juce::String("Off");
                return juce::String(juce::roundToInt(value)) + " Hz";
            },
            [](const juce::String& text) {
                if (text.compareIgnoreCase("off") == 0)
                    return 20000.0f;
                return text.getFloatValue();
            }
        ));
        
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID{"MODE", 1}, "Mode",
            juce::StringArray{"Off", "Clean", "Further", "Crunchy"}, 0));
        
        return { params.begin(), params.end() };
    }

private:

    std::unique_ptr<Service::PresetManager> presetManager;

    // DSP Processor
    DSP::FloatProcessor dspProcessor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
