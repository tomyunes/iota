#pragma once

#include "ofMain.h"
#include "Controller.h"
#include "GUIManager.h"

class GamemodeManager
{
public:

	GamemodeManager(int game_mode_id = 0);
	void init(GUIManager* gui_manager);

	void update();

	void draw();
	
	int get_current_mode_id() const;
	string get_current_mode_string() const;
	void set_current_mode_id(int game_mode_id);
	void log_current_mode() const;

	
	void set_request_for_main_mode(const bool req) { request_for_main_mode_ = req; }
	bool get_request_for_main_mode() const { return request_for_main_mode_; }
	
	void set_request_for_procedural_scene(const bool req) { request_for_procedural_scene_ = req; }
	bool get_request_for_procedural_scene() const { return request_for_procedural_scene_;  }

	void set_request_for_blank_scene(const bool req) { request_for_blank_scene_ = req; }
	bool get_request_for_blank_scene() const { return request_for_blank_scene_; }

	void set_is_transitioning(const bool val) { transitioning_ = val;  }
	bool get_is_transitioning() const { return transitioning_; }

	void set_main_mode_started(const bool val) { main_mode_started_ = val; }
	bool get_main_mode_started() const { return main_mode_started_; }

	int get_prev_gamemode() const { return prev_mode_id_; }
	
	
	void scene_load_fade();
	void transition_scene();

	void key_pressed(int key);
	void mouse_pressed(int x, int y, int button);
	
	bool game_started_;	

private:

	GUIManager* gui_manager_;
	
	int current_mode_id_;
	int prev_mode_id_;

	bool request_for_main_mode_;
	bool request_for_procedural_scene_;
	bool request_for_blank_scene_;

	bool fading_;
	bool transitioning_;
	float fill_alpha_;
	float text_alpha1_;
	float text_alpha2_;
	bool transitioning_text1_;
	bool transitioning_text2_;
	float frame_before_transition_{};

	bool main_mode_started_;

	ofTrueTypeFont potta_one_main_;

};
