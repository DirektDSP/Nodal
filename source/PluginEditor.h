#pragma once

#include "BinaryData.h"
#include "PluginProcessor.h"
#include "UI/Components/AnimatedBackground.h"
#include "UI/Components/PresetPanel.h"
#include "UI/Components/RasterKnob.h"
#include "UI/Components/SmallerRasterKnob.h"
#include "UI/Components/ModeCB.h"
#include "UI/Utils/Timestamp.h"
#include "melatonin_inspector/melatonin_inspector.h"

#include <juce_gui_basics/juce_gui_basics.h>

// Include the Moonbase Activation UI header (adjust path if needed)
#include "moonbase_JUCEClient/moonbase_JUCEClient.h"

class PluginEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override; //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    AudioProcessorValueTreeState& apvts;

    // Helper functions for UI setup
    PluginProcessor& processorRef;

    void sliderValueChanged(juce::Slider* slider) override;


    // A button to show a sample inspector (if needed)
    juce::TextButton inspectButton { "Inspect the UI" };

    TimestampLabel timestampLabel;

    // keep aspect ratio when resizing :)
    juce::ComponentBoundsConstrainer constrainer;

    // Licensing activation UI
    std::unique_ptr<Moonbase::JUCEClient::ActivationUI> activationUI;

    // Optional: a sample inspector from the melatonin module
    std::unique_ptr<melatonin::Inspector> inspector;
    // Actual Plugin UI
    UI::Components::PresetPanel presetPanel;

    // DSP Parameter Controls
    UI::Components::RasterKnob delaySlider;

    // APVTS Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
