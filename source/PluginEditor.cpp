#include "PluginEditor.h"
#include "UI/Utils/CustomFont.h"

int TEXT_BOX_WIDTH = 80;
int TEXT_BOX_HEIGHT = 20;

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), presetPanel(p.getPresetManager()), apvts(p.getApvts())
{
    // Create the activation UI via the Moonbase client.
    // The activation UI is created using the licensing member from the processor.
    if (processorRef.moonbaseClient != nullptr)
        activationUI.reset(processorRef.moonbaseClient->createActivationUi(*this));


    // Customize the activation UI if it exists.
    if (activationUI)
    {
        // Set welcome text (max 2 lines) for the activation screen.
        activationUI->setWelcomePageText ("PluginTemplate", "Made by Korzana & DirektDSP");

        // Set spinner logo from your BinaryData assets.

        //not using, it looks ugly, might change later
        // activationUI->setSpinnerLogo (juce::Drawable::createFromImageData (BinaryData::direktdsp_svg, BinaryData::direktdsp_svgSize));
        // Optionally set company logo (replace CompanyLogo with your drawable class).
        // activationUI->setCompanyLogo (std::make_unique<CompanyLogo>());
    }

    addAndMakeVisible (timestampLabel);
    timestampLabel.setText ("Chasm_Build_" + String(__DATE__) + "_" + String(__TIME__), juce::dontSendNotification);

    // Add a button to show an inspector (example usage of another module).
    #ifdef JUCE_DEBUG
        addAndMakeVisible (inspectButton);
        inspectButton.onClick = [&] {
            if (!inspector)
            {
                inspector = std::make_unique<melatonin::Inspector> (*this);
                inspector->onClose = [this]() { inspector.reset(); };
            }
            inspector->setVisible (true);
        };
    #endif

    constrainer.setFixedAspectRatio (2000.0f/1200.0f);

    // Now tell the editor to use this constrainer
    setConstrainer (&constrainer);

    // Allow the editor to be resizable
    setResizable(false, true);
    constrainer.setMinimumSize(800, 420);
    addAndMakeVisible(presetPanel);

    setSize (400, 300);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION +
                      " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    inspectButton.setBounds (area.removeFromBottom (50).withSizeKeepingCentre (100, 50));

    // IMPORTANT: Ensure the activation UI is resized as well.
    MOONBASE_RESIZE_ACTIVATION_UI
}
//==============================================================================

void PluginEditor::sliderValueChanged(juce::Slider* slider)
{
    float delay = apvts.getRawParameterValue("DELAY")->load();
    float character = apvts.getRawParameterValue("CHARACTER")->load();

    /*
    if (slider == &delaySlider) {
    }
    
    */
}

