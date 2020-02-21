/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class Fib_seqAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Fib_seqAudioProcessor();
    ~Fib_seqAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

	AudioParameterInt * mNoteMin;
	AudioParameterInt* mNoteRange;
	AudioParameterInt * mVeloMin;
	AudioParameterInt* mVeloRange;
	AudioParameterFloat* mGate;
	AudioParameterInt* mInterval;
	AudioParameterInt* mDivision; //need to cast to double(pow(x,2))

	int mNote1 = 60;
	int mNote2 = 61;
	int mNotef;
	int mVelo1 = 63;
	int mVelo2 = 64;
	int mVelof;
	int noteOnBuffIndex; //for dealing with non-zero note-on command indeces in the buffer
	int noteOffBuffIndex; //for dealing with non-zero note-off command indeces in the buffer
	double rate;
	double beatsPerBuffer;
	double mBPM;
	double currentPing;
	double nextPing;
	double noteEnd; //has to happen sooner than the next noteEND by like a sample or 4.
	bool firstPlayed = false; // dealing with starting directly on the 1/4 note
	bool notePlaying = false; //dealing with if statements between midi on or off commands WITHIN a buffer. reeee.
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fib_seqAudioProcessor)
};
