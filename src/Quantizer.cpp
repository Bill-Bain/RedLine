#include "plugin.hpp"

//
struct Quantizer : Module {
	enum ParamIds {
		SCALE_PARAM,
		ON_PARAM,
		TYPE_PARAM,
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

	float chrom_table [13] = {0,.0833,.1667,.25,.3333,.4167,.5,.5833,.6667,.75,.83333,.9167,1};
	float maj_table [13] = {0.00001,.1667,.33334,.4167,.58334,.75,.9167,1,0,0,0,0,0};
	float min_table [13] = {0,.1667,.25,.4167,.5833,.75,.8333,1,0,0,0,0,0};

	float scale_table [4] [13] =
	 {0.,.08334,.1667,.25,.33334,.4167,.5,.58334,.6667,.75,.83334,.9167,1.,
	  0.,.1667,.25,   .4167,.58334,.75,.83334,1.,0,0,0,0,0,
	  0.,.1667,.33334,.4167,.58334,.75,.9167,1.,0,0,0,0,0,
	  0.,.25, .4167, .5, .58334, .83334, 1.0, 0,0,0,0,0,0};
 
	Quantizer() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(SCALE_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ON_PARAM, 0, 1, 1, "");
		configParam(TYPE_PARAM, 0, 3, 2, "");
	}

	void process(const ProcessArgs& args) override {
/*
            if enabled {
		get in cv
		compare it to table values
		set output cv
            } 
*/
            /* written thurs apr 23 2020 ( was dated jun 12 2020) */

            int scale = params[TYPE_PARAM].getValue();

            float note ;
            float cv_out = 0;
	    float cv_in = inputs[CV_INPUT].getVoltage();
            float On = params[ON_PARAM].getValue();
            lights[ENABLED_LIGHT].setBrightness(On == 1.f ? 1.f : 0.f);

	    if (On == 1.f) {
                note = cv_in - int(cv_in);
		int i = 1;
                while (i <= 13) {
                    if (note <= scale_table[scale][i]) {
			cv_out = cv_in - note + scale_table[scale][i];
                        outputs[CV_OUTPUT].setVoltage(cv_out);
		        break;
                    }
                    i ++;
                }                 
            }
            else {
                outputs[CV_OUTPUT].setVoltage(cv_in);
            }

	}
};


struct QuantizerWidget : ModuleWidget {
	QuantizerWidget(Quantizer* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Quantizer.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
//		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
//		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundHugeBlackKnob>(mm2px(Vec(15.505, 38.299)), module, Quantizer::SCALE_PARAM));
		addParam(createParamCentered<CKSS>(mm2px(Vec(15.505, 61.075)), module, Quantizer::ON_PARAM));
		addParam(createParamCentered<BBSN>(mm2px(Vec(7.567, 80.368)), module, Quantizer::TYPE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.26, 112.008)), module, Quantizer::CV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.225, 112.008)), module, Quantizer::CV_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(6.879, 61.075)), module, Quantizer::ENABLED_LIGHT));
	}
};


Model* modelQuantizer = createModel<Quantizer, QuantizerWidget>("Quantizer");
