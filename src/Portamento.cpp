#include "plugin.hpp"

//
struct Portamento : Module {
	enum ParamIds {
		TIME_PARAM,
		ON_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
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

        float last_cv[16] = {999.f};


	Portamento() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(TIME_PARAM, .00001f, 10.f, .00001f, "glide time");
		configParam(ON_PARAM, 0.f, 1.f, 0.f, "enable");
	}




void process(const ProcessArgs& args) override {
	// written evening April 18 2020, poly mod aug 29/21
/*
	assume Time is seconds per octave
	cv-out increases each time until it matches cv-in
	need to save curr cv-out and cv-in
	when cv-in changes calc cv change factor then add it each pass
*/


	int channels = inputs[CV_INPUT].getChannels();
	outputs[CV_OUTPUT].setChannels(channels);	// set output channels

        float time = params[TIME_PARAM].getValue();	// glide speed

        float On = params[ON_PARAM].getValue();
        lights[ENABLED_LIGHT].setBrightness(On == 1.f ? 1.f : 0.f);


        for (int c = 0; c < channels; c++) {
            float cv_out = 0;
	    float cv_in = inputs[CV_INPUT].getVoltage(c);

	    if (On == 1.f) {
                float delta_cv = 0.f;

                if (last_cv[c] == 999.f) last_cv[c] = cv_in;	// first time

	        if (cv_in != last_cv[c]) {
		    delta_cv = args.sampleTime / time;		// calculate step value
                    if (cv_in < last_cv[c]) delta_cv = -delta_cv ;	// invert if down
                    cv_out = last_cv[c] + delta_cv;		// increment cv
                    last_cv[c] = cv_out;
  
                    if (delta_cv > 0 and cv_out > cv_in) 	// prevent overshoot
	                cv_out = cv_in;
                    if (delta_cv < 0 and cv_out < cv_in) 
		        cv_out = cv_in;
                }
	        else {
                    cv_out = cv_in;
                }

                last_cv[c] = cv_out;				// save for next slice
                // set the cv output
                outputs[CV_OUTPUT].setVoltage(cv_out, c);
            }
            else {						// module disabled
                cv_out = cv_in;
                outputs[CV_OUTPUT].setVoltage(cv_out, c);
            }
	}
    }

};

/*
	void process(const ProcessArgs& args) override {
		// written evening April 18 2020
/ *
	assume Time is seconds per octave
	cv-out increases each time until it matches cv-in
	need to save curr cv-out and cv-in
	when cv-in changes calc cv change factor then add it each pass
*/
/*
            float cv_out = 0;
            float cv_in = inputs[CV_INPUT].getVoltage();
            float On = params[ON_PARAM].getValue();
        lights[ENABLED_LIGHT].setBrightness(On == 1.f ? 1.f : 0.f);
	if (On == 1.f) {
	    float time = params[TIME_PARAM].getValue() + .00001f; // prevent div by 0
            float delta_cv = 0.f;

            if (last_cv == 999.f) last_cv = cv_in;

	        if (cv_in != last_cv) {
		    delta_cv = args.sampleTime / time;		// calculate step value
                    if (cv_in < last_cv) delta_cv = -delta_cv ;
                    cv_out = last_cv + delta_cv;		// output incremented cv
                    last_cv = cv_out;
  
                    if (delta_cv > 0 and cv_out > cv_in) 	// prevent overshoot
	                cv_out = cv_in;
                    if (delta_cv < 0 and cv_out < cv_in) 
		        cv_out = cv_in;
                }
	        else {
                    cv_out = cv_in;
                }

                last_cv = cv_out;				// save for next slice
                // set the cv output
                outputs[CV_OUTPUT].setVoltage(cv_out);
            }
            else {
                cv_out = cv_in;
                outputs[CV_OUTPUT].setVoltage(cv_out);
            }
	}
};
*/

struct PortamentoWidget : ModuleWidget {
	PortamentoWidget(Portamento* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Portamento.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
//		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
//		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundHugeBlackKnob>(mm2px(Vec(15.505, 38.299)), module, Portamento::TIME_PARAM));
		addParam(createParamCentered<CKSS>(mm2px(Vec(15.505, 61.075)), module, Portamento::ON_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.26, 112.008)), module, Portamento::CV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.225, 112.008)), module, Portamento::CV_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(6.879, 61.075)), module, Portamento::ENABLED_LIGHT));
	}
};


Model* modelPortamento = createModel<Portamento, PortamentoWidget>("Portamento");
