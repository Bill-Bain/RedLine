#include "plugin.hpp"


struct Arpegiator : Module {
	enum ParamIds {
		THREEFOUR_PARAM,
		ENABLED_PARAM,
		UPDOWN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CLOCK_INPUT,
		CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		CV_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENABLED_LIGHT,
		NUM_LIGHTS
	};

	// session persistant variables
	// note tables
	float notes [5] = {0.f, .3333f, .5833f, .8333f, 1.1667f};
	int currentnote = 0;	// table index
	bool goingup = true;


	dsp::SchmittTrigger clock;

		


	Arpegiator() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(THREEFOUR_PARAM, 0, 2, 0, "");
		configParam(ENABLED_PARAM, 0, 1, 1, "");
		configParam(UPDOWN_PARAM, 0, 1, 0, "");
	}

	void process(const ProcessArgs& args) override {
	// started May 8 2020 working 10am 9th

	    bool enabled = params[ENABLED_PARAM].getValue();
            lights[ENABLED_LIGHT].setBrightness(enabled == 1.f ? 1.f : 0.f);
	    bool updown = ! params[UPDOWN_PARAM].getValue();
	    int numsteps = params[THREEFOUR_PARAM].getValue() + 2;

	    float cv = inputs[CV_INPUT].getVoltage();

/*
(clockTrigger.process(inputs[EXT_CLOCK_INPUT].getVoltage())) {
					setIndex(index + 1);
				}
				gateIn = clockTrigger.isHigh();
			}
*/

	    if (enabled) {
		if (clock.process(inputs[CLOCK_INPUT].getVoltage())) {
		    // consider direction
		    if (goingup and currentnote >= numsteps) {
		        if (updown) {
		            currentnote = numsteps - 1;
		            goingup = ! goingup;
                        }
                        else currentnote = 0;
		    }
 		    else if (! goingup and currentnote == 0) {
		        currentnote += 1;
		        goingup = ! goingup;    
		    }
		    else if (goingup) currentnote += 1;
		           else currentnote -= 1;

		    // calculate next note
		    float note = cv + notes[currentnote] ;
                    outputs[CV_OUTPUT].setVoltage(note);
		}

	    }
	    else outputs[CV_OUTPUT].setVoltage(cv);
	}
};


struct ArpegiatorWidget : ModuleWidget {
	ArpegiatorWidget(Arpegiator* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Arpegiator.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
//		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
//		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<NKK>(mm2px(Vec(15.505, 38.299)), module, Arpegiator::THREEFOUR_PARAM));
		addParam(createParamCentered<CKSS>(mm2px(Vec(15.505, 54.417)), module, Arpegiator::ENABLED_PARAM));
		addParam(createParamCentered<NK2>(mm2px(Vec(15.505, 70.335)), module, Arpegiator::UPDOWN_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.285, 94.369)), module, Arpegiator::CLOCK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.306, 112.008)), module, Arpegiator::CV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.696, 112.008)), module, Arpegiator::CV_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(5.556, 54.725)), module, Arpegiator::ENABLED_LIGHT));
	}
};


Model* modelArpegiator = createModel<Arpegiator, ArpegiatorWidget>("Arpegiator");
