/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Fib_seqAudioProcessor::Fib_seqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	addParameter(mNoteMin = new AudioParameterInt("minNote", "Minimum Note", 1, 90, 46));
	addParameter(mNoteRange = new AudioParameterInt("rangeNote", "Note Range", 4, 37, 20));
	addParameter(mGate = new AudioParameterFloat("gate", "Gate Percentage", 0.03, .98, 0.98));
	addParameter(mInterval = new AudioParameterInt("interval", "Interval", 1, 16, 1));
	addParameter(mDivision = new AudioParameterInt("div", "Division", 0, 2, 0));
	addParameter(mVeloMin = new AudioParameterInt("minVelo", "Minimum Velocity", 1, 127, 63));
	addParameter(mVeloRange = new AudioParameterInt("VeloRange", "Velocity Range", 1, 63, 31));
}

Fib_seqAudioProcessor::~Fib_seqAudioProcessor()
{
}

//==============================================================================
const String Fib_seqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Fib_seqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Fib_seqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Fib_seqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Fib_seqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Fib_seqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Fib_seqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Fib_seqAudioProcessor::setCurrentProgram (int index)
{
}

const String Fib_seqAudioProcessor::getProgramName (int index)
{
    return {};
}

void Fib_seqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Fib_seqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Fib_seqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Fib_seqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void Fib_seqAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midi)
{
	jassert(buffer.getNumChannels() == 0);
	const int divisions[] = { 0, 1, 2 };

	int buff = buffer.getNumSamples(); //samples/buffer
	double rate = getSampleRate(); //samples/second
	AudioPlayHead *head = getPlayHead();
	AudioPlayHead::CurrentPositionInfo positionInfo;
	head->getCurrentPosition(positionInfo);
	mBPM = positionInfo.bpm;
	double currentPosition = positionInfo.ppqPosition; //use this as the floating ticker
	double beatsPerBuffer = buff * mBPM / 60.0 / rate;
	double div = pow(2, divisions[*mDivision]);
	nextPing = currentPing + *mInterval / div;
	noteEnd = currentPing + *mGate / div;

	if (positionInfo.isPlaying)
	{
		if (firstPlayed == false) //this deals with starting conditions
		{
			if (currentPosition == floor(currentPosition)) //the case where we are directly ON the quarter note
			{
				mNotef = (((mNote1 + mNote2) - *mNoteMin) % *mNoteRange) + *mNoteMin;
				mVelof = jmin((((mVelo1 + mVelo2) - *mVeloMin) % *mVeloRange) + *mVeloMin, 127);
				firstPlayed = true;
				notePlaying = true;
				midi.addEvent(MidiMessage::noteOn(1, mNotef, uint8(mVelof)), 0);
				currentPing = currentPosition;
				noteEnd = currentPing + *mGate / div;
				mNote1 = mNote2;
				mNote2 = mNotef;
				mVelo1 = mVelo2;
				mVelo2 = mVelof;
				nextPing = currentPing + *mInterval / div;
			}

			else if (currentPosition + beatsPerBuffer >= floor(currentPosition) + 1) //the case where we need to catch up to the next quarter note
			{
				mNotef = (((mNote1 + mNote2) - *mNoteMin) % *mNoteRange) + *mNoteMin;
				mVelof = jmin((((mVelo1 + mVelo2) - *mVeloMin) % *mVeloRange) + *mVeloMin, 127);
				firstPlayed = true;
				notePlaying = true;
				currentPing = ceil(currentPosition);
				noteEnd = currentPing + *mGate / div;
				noteOnBuffIndex = (currentPing - currentPosition) / beatsPerBuffer * buff + 0.5; //target units are in samples. starting with ppq
				midi.addEvent(MidiMessage::noteOn(1, mNotef, uint8(mVelof)), noteOnBuffIndex);
				mNote1 = mNote2;
				mNote2 = mNotef;
				mVelo1 = mVelo2;
				mVelo2 = mVelof;
				nextPing = currentPing + *mInterval / div;
			}
		}

		if (currentPosition + beatsPerBuffer >= noteEnd && notePlaying == true)
		{
			notePlaying = false;
			noteOffBuffIndex = (noteEnd - currentPosition) / beatsPerBuffer * buff + 0.5;
			midi.addEvent(MidiMessage::noteOff(1, mNotef, uint8(mVelof)), noteOffBuffIndex);
		}

		if (currentPosition - nextPing >= 1)  //deals with the 3,5,7 case that I described on Rob's wall
		{
			nextPing = floor(currentPosition) + float(1 / div);
		}

		if (currentPosition + beatsPerBuffer >= nextPing && notePlaying == false)
		{
			notePlaying = true;
			mNotef = (((mNote1 + mNote2) - *mNoteMin) % *mNoteRange) + *mNoteMin;
			mVelof = jmin((((mVelo1 + mVelo2) - *mVeloMin) % *mVeloRange) + *mVeloMin, 127);
			currentPing = nextPing;
			noteEnd = currentPing + *mGate / div;
			noteOnBuffIndex = (nextPing - currentPosition) / beatsPerBuffer * buff + 0.5;
			midi.addEvent(MidiMessage::noteOn(1, mNotef, uint8(mVelof)), noteOnBuffIndex);
			mNote1 = mNote2;
			mNote2 = mNotef;
			mVelo1 = mVelo2;
			mVelo2 = mVelof;
			nextPing = currentPing + *mInterval / div;
		}

	}
	else
	{
		midi.clear();
		firstPlayed = false;
		notePlaying = false;
	}
}

//==============================================================================
bool Fib_seqAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Fib_seqAudioProcessor::createEditor()
{
    return new Fib_seqAudioProcessorEditor (*this);
}

//==============================================================================
void Fib_seqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Fib_seqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Fib_seqAudioProcessor();
}
