#pragma once
#ifndef PANELED_UI_H
#define PANELED_UI_H

#include "output.h"
#include "enums.h"
#include "cursesdef.h"

#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

namespace ui
{
class padding_panel;

enum border_direction
{
	nothing = 0,
	north = 1 << 0,
	east = 1 << 1,
	south = 1 << 2,
	west = 1 << 3,
	clear = 1 << 4 // Clears border state
};

class window;

typedef std::vector<std::vector<border_direction>> border_direction_vector;
typedef std::vector<std::vector<nc_color>> border_color_vector;
class border_manager
{
public:
	border_manager(point size);

	void draw(window& win);
	void add_border_data(border_direction_vector border_dir, border_color_vector border_color);
	void fix_borders();
private:
	border_direction_vector border_dir_data;
	border_color_vector border_color_data;
};

namespace utils
{
	void mark_border
	(
		point start, point size,
		border_direction_vector& border_dir,
		border_color_vector& border_color,
		bool clear_internal, nc_color color
	);
	void mark_line
	(
		point start, unsigned int length,
		border_direction_vector& border_dir,
		border_color_vector& border_color,
		bool vertical, nc_color color
	);
}


class panel
{
public:
	panel() {}
	virtual ~panel() {};

	virtual void draw() = 0;

	virtual point min_size() = 0;
	virtual point perfered_size() = 0;

	virtual point min_size(int size, bool hieght) = 0;
	virtual point perfered_size(int size, bool hieght) = 0;

	virtual void set_size(point size, point offset) = 0;

	virtual bool is_bordered() = 0;

	void set_parent(panel* parent) { this->parent = parent; }
	void set_owner(window* win) { this->owner = win; }
protected:
	panel* parent;
	window* owner;
};

class debug_panel2 : public panel
{
	void draw() {}
	point min_size() { return {11, 10}; }
	point perfered_size() { return {20, 25}; }

	bool is_bordered() { return true; };

	void set_size(point size, point offset);

	point min_size(int size, bool hieght) { return {(hieght ? min_size().x : size), (hieght ? size : min_size().y)}; };
	point perfered_size(int size, bool hieght) { return {(hieght ? perfered_size().x : size), (hieght ? size : perfered_size().y)}; };
};

class debug_panel : public panel
{
	void draw() {}
	point min_size() { return {5, 5}; }
	point perfered_size() { return {50, 50}; }
	bool is_bordered() { return true; };

	void set_size(point size, point offset);

	point min_size(int size, bool hieght) { return {(hieght ? min_size().x : size), (hieght ? size : min_size().y)}; };
	point perfered_size(int size, bool hieght) { return {(hieght ? perfered_size().x : size), (hieght ? size : perfered_size().y)}; };
};

class padding_panel : public panel
{
public:
	padding_panel() { padding_panel(false); }
	padding_panel(bool bordered);

	void draw();
	std::unique_ptr<panel> swap_child(std::unique_ptr<panel> child)
	{
		child->set_owner(owner);
		child->set_parent(this);
		std::swap(this->child, child);
		return child;
	}

	panel* get_child() { return child.get(); }

	point min_size();
	point perfered_size();
	point min_size(int size, bool hieght);
	point perfered_size(int size, bool hieght);
	void set_size(point size, point offset);

	bool is_bordered() { return bordered; }
private:
	bool bordered;
	std::unique_ptr<panel> child;
	point size;
	point offset;
};

class split_panel : public panel
{
public:
	split_panel() { split_panel(false, true, false); }
	split_panel(bool bordered, bool entries_seperated, bool stacked);

	void draw();
	void add_child(std::unique_ptr<panel> child)
	{
		child->set_parent(this);
		child->set_owner(owner);
		children.push_back(std::move(child));
	}

	const std::vector<std::unique_ptr<panel>>& get_children() const
	{
		return children;
	}

	point perfered_size();
	point min_size();
	point min_size(int size, bool hieght);
	point perfered_size(int size, bool hieght);
	void set_size(point size, point offset);

	bool is_bordered() { return bordered; }

private:
	void update_sizes();

	bool bordered;
	bool stacked;
	bool entries_seperated;
	std::vector<point> sizes;
	std::vector<std::unique_ptr<panel>> children;
	point size;
	point offset;
};

class label_panel : public panel
{
public:
	label_panel() { label_panel(false, 100); }
	label_panel(bool bordered, size_t expected_text_amount);

	void set_label(std::string label) { this->label = label; }
	std::string get_label() { return label; }

	void set_expected_text_amount(int expected_text_amount) { this->expected_text_amount = expected_text_amount; }
	int get_expected_text_amount() { return expected_text_amount; }

	void draw();
	point min_size();
	point perfered_size();
	point min_size(int size, bool hieght);
	point perfered_size(int size, bool hieght);
	void set_size(point size, point offset);

	bool is_bordered() { return bordered; }
private:
	std::string label;
	bool bordered;
	int expected_text_amount;
	point size;
	point offset;
};

class window
{
public:
	window();

	void draw();
	std::unique_ptr<padding_panel> swap_child(std::unique_ptr<padding_panel> child)
	{
		child->set_owner(this);
		child->set_parent(nullptr);
		std::swap(this->child, child);
		return child;
	}

	padding_panel* get_child() { return child.get(); };

	const catacurses::window& get_window() const { return win; }

	// TODO: Remove eventualy, for legacy support
	catacurses::window& lwin() { return win; }

	point min_size();
	point perfered_size();
	void set_size(point size);

	std::unique_ptr<border_manager> border_handler;
private:
	std::unique_ptr<padding_panel> child;
	point size;
	point offset;
	catacurses::window win;
};

}
#endif
