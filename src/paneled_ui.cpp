#include "paneled_ui.h"

	void ui::debug_panel2::set_size(point size, point offset)
	{
		border_direction_vector border_dir
		(
			offset.x + size.x,
			std::vector<border_direction>(offset.y + size.y, ui::border_direction::nothing)
		);
		border_color_vector border_color
		(
			offset.x + size.x,
			std::vector<nc_color>(offset.y + size.y, nc_color())
		);
		utils::mark_border(offset, size, border_dir, border_color, true, c_red);
		owner->border_handler->add_border_data(border_dir, border_color);
	}
	void ui::debug_panel::set_size(point size, point offset)
	{
		border_direction_vector border_dir
		(
			offset.x + size.x,
			std::vector<border_direction>(offset.y + size.y, ui::border_direction::nothing)
		);
		border_color_vector border_color
		(
			offset.x + size.x,
			std::vector<nc_color>(offset.y + size.y, nc_color())
		);
		utils::mark_border(offset, size, border_dir, border_color, true, c_red);
		owner->border_handler->add_border_data(border_dir, border_color);
	}

ui::border_manager::border_manager(point size)
	: border_dir_data(size.x, std::vector<border_direction>(size.y, ui::border_direction::nothing)),
	border_color_data(size.x, std::vector<nc_color>(size.y, nc_color())) {}

void ui::border_manager::add_border_data(border_direction_vector border_dir, border_color_vector border_color)
{
	assert(border_dir.size() <= border_dir_data.size());
	assert(border_color.size() <= border_color_data.size());
	size_t i = 0;
	for (auto& bvs : border_dir)
	{
		assert(bvs.size() <= border_dir_data[i].size());
		++i;
	}
	i = 0;
	for (auto& bvs : border_color)
	{
		assert(bvs.size() <= border_color_data[i].size());
		++i;
	}

	i = 0;
	for (auto& bvs : border_dir)
	{
		size_t j = 0;
		for (auto& tile : bvs)
		{
			if ((tile & ui::border_direction::clear) != ui::border_direction::clear)
			{
				border_dir_data[i][j] = (border_direction)(border_dir_data[i][j] | tile);
			}
			else
			{
				border_dir_data[i][j] = ui::border_direction::nothing;
			}
			++j;
		}
		++i;
	}

	i = 0;
	for (auto& bvs : border_color)
	{
		size_t j = 0;
		for (auto& tile : bvs)
		{
			if ((int)tile != 0)
			{
				border_color_data[i][j] = tile;
			}
			++j;
		}
		++i;
	}
}

void ui::border_manager::draw(window& win)
{
	auto x = 0;
	for (auto& bvs : border_dir_data)
	{
		auto y = 0;
		for (auto& tile : bvs)
		{
			auto OOXX = ui::border_direction::south | ui::border_direction::west;
			auto OXOX = ui::border_direction::east  | ui::border_direction::west;
			auto OXXO = ui::border_direction::east  | ui::border_direction::south;
			auto XOOX = ui::border_direction::north | ui::border_direction::west;
			auto XOXO = ui::border_direction::north | ui::border_direction::south;
			auto XXOO = ui::border_direction::north | ui::border_direction::east;
			auto OXXX = OXXO | OOXX;
			auto XOXX = XOXO | XOOX;
			auto XXOX = XXOO | OXOX;
			auto XXXO = XXOO | OXXO;
			auto XXXX = XXOX | XXXO;
			if (tile == ui::border_direction::nothing) {}
			else if (tile == OOXX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_OOXX);
			}
			else if (tile == OXOX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_OXOX);
			}
			else if (tile == OXXO)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_OXXO);
			}
			else if (tile == XOOX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XOOX);
			}
			else if (tile == XOXO)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XOXO);
			}
			else if (tile == XXOO)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XXOO);
			}
			else if (tile == OXXX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_OXXX);
			}
			else if (tile == XOXX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XOXX);
			}
			else if (tile == XXOX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XXOX);
			}
			else if (tile == XXXO)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XXXO);
			}
			else if (tile == XXXX)
			{
				mvwputch(win.get_window(), y, x, border_color_data[x][y], LINE_XXXX);
			}
			else
			{
				//throw std::runtime_error("Invalid border tile");
			}
			++y;
		}
		++x;
	}
}

void ui::border_manager::fix_borders()
{
	size_t i = 0;
	for (auto& bvs : border_dir_data)
	{
		size_t j = 0;
		for (auto& tile : bvs)
		{
			if (tile == 0)
			{
				j++;
				continue;
			}

			if ((j + 2) <= bvs.size())
			{
				if ((border_dir_data[i][j + 1] & ui::border_direction::north) == ui::border_direction::north)
				{
					tile = (border_direction)(tile | ui::border_direction::south);
				}
			}

			if (j > 0)
			{
				if ((border_dir_data[i][j - 1] & ui::border_direction::south) == ui::border_direction::south)
				{
					tile = (border_direction)(tile | ui::border_direction::north);
				}
			}

			if ((i + 2) <= border_dir_data.size())
			{
				if ((border_dir_data[i + 1][j] & ui::border_direction::west) == ui::border_direction::west)
				{
					tile = (border_direction)(tile | ui::border_direction::east);
				}
			}

			if (i > 0)
			{
				if ((border_dir_data[i - 1][j] & ui::border_direction::east) == ui::border_direction::east)
				{
					tile = (border_direction)(tile | ui::border_direction::west);
				}
			}

			++j;
		}
		++i;
	}
}

void ui::utils::mark_line
(
	point start, unsigned int length,
	border_direction_vector& border_dir,
	border_color_vector& border_color,
	bool vertical, nc_color color
)
{
	if (length == 0)
	{
		return;
	}

	auto l2 = length;
	auto s2 = start;
	auto line =
	(
		vertical ?
		((ui::border_direction)(ui::border_direction::north | ui::border_direction::south)) :
		((ui::border_direction)(ui::border_direction::east | ui::border_direction::west))
	);

	while (l2 != 0)
	{
		border_dir[s2.x][s2.y] = line;
		border_color[s2.x][s2.y] = color;
		--l2;
		if (vertical)
		{
			++s2.y;
		}
		else
		{
			++s2.x;
		}
	}
}

void ui::utils::mark_border
(
	point start, point size,
	border_direction_vector& border_dir,
	border_color_vector& border_color,
	bool clear_internal, nc_color color
)
{
	size -= {1, 1};

	// corners
	border_dir[start.x][start.y] = (ui::border_direction)(ui::border_direction::east | ui::border_direction::south);
	border_dir[start.x + size.x][start.y] = (ui::border_direction)(ui::border_direction::west | ui::border_direction::south);

	border_dir[start.x][start.y + size.y] = (ui::border_direction)(ui::border_direction::east | ui::border_direction::north);
	border_dir[start.x + size.x][start.y + size.y] = (ui::border_direction)(ui::border_direction::west | ui::border_direction::north);

	border_color[start.x][start.y] = color;
	border_color[start.x + size.x][start.y] = color;

	border_color[start.x][start.y + size.y] = color;
	border_color[start.x + size.x][start.y + size.y] = color;

	for(int stx = start.x + 1; stx < start.x + size.x; ++stx)
	{
		border_dir[stx][start.y] = (ui::border_direction)(ui::border_direction::east | ui::border_direction::west);
		border_dir[stx][start.y + size.y] = (ui::border_direction)(ui::border_direction::east | ui::border_direction::west);
		border_color[stx][start.y] = color;
		border_color[stx][start.y + size.y] = color;
	}

	for(int sty = start.y + 1; sty < start.y + size.y; ++sty)
	{
		border_dir[start.x][sty] = (ui::border_direction)(ui::border_direction::north | ui::border_direction::south);
		border_dir[start.x + size.x][sty] = (ui::border_direction)(ui::border_direction::north | ui::border_direction::south);
		border_color[start.x][sty] = color;
		border_color[start.x + size.x][sty] = color;
	}

	if (clear_internal)
	{
		for(int stx = start.x + 1; stx < start.x + size.x; ++stx)
		{
			for(int sty = start.y + 1; sty < start.y + size.y; ++sty)
			{
				border_dir[stx][sty] = (ui::border_direction)(ui::border_direction::clear);
				border_dir[stx][sty] = (ui::border_direction)(ui::border_direction::clear);
			}
		}
	}
}

ui::window::window()
{
	child = std::make_unique<padding_panel>(true);
	child->set_owner(this);
	child->set_parent(nullptr);
}

void ui::window::draw()
{
	assert(child);
	child->draw();
	border_handler->draw(*this);

	wrefresh(win);
}

point ui::window::perfered_size()
{
	assert(child);
	return child->perfered_size();
}

point ui::window::min_size()
{
	assert(child);
	return child->min_size();
}

void ui::window::set_size(point size)
{
	assert(child);
	assert(size.x >= min_size().x);
	assert(size.y >= min_size().y);

	this->size = size;

	offset.x = (TERMX > size.x ? (TERMX - size.x) / 2 : 0);
	offset.y = (TERMY > size.y ? (TERMY - size.y) / 2 : 0);

	win = catacurses::newwin(size.y, size.x, offset.y, offset.x);
	border_handler = std::make_unique<border_manager>(size);
	child->set_size(size, {0, 0});
	border_handler->fix_borders();
}

ui::padding_panel::padding_panel(bool bordered) : bordered(bordered) {}

void ui::padding_panel::draw()
{
	if (child)
	{
		child->draw();
	}
}

point ui::padding_panel::perfered_size()
{
	point ret = {0, 0};
	if (bordered)
	{
		ret = {1, 1};
	}

	if (child)
	{
		ret += child->perfered_size();
	}

	if (ret.x == 0 || ret.y == 0)
	{
		ret = {1, 1};
	}

	return ret;
}

point ui::padding_panel::min_size()
{
	point ret = {0, 0};
	if (bordered)
	{
		ret = {2, 2};
	}

	if (child)
	{
		ret += child->min_size();
	}

	if (ret.x == 0 || ret.y == 0)
	{
		ret = {1, 1};
	}

	return ret;
}

void ui::padding_panel::set_size(point size, point offset)
{
	this->size = size;
	this->offset = offset;

	if (bordered)
	{
		border_direction_vector border_dir
		(
			offset.x + size.x,
			std::vector<border_direction>(offset.y + size.y, ui::border_direction::nothing)
		);
		border_color_vector border_color
		(
			offset.x + size.x,
			std::vector<nc_color>(offset.y + size.y, nc_color())
		);
		utils::mark_border(offset, size, border_dir, border_color, true, BORDER_COLOR);
		owner->border_handler->add_border_data(border_dir, border_color);
		size -= {2, 2};
		offset += {1, 1};
	}

	if (child)
	{
		child->set_size(size, offset);
	}
}

ui::split_panel::split_panel(bool bordered, bool entries_seperated, bool stacked)
	: bordered(bordered), stacked(stacked), entries_seperated(entries_seperated) {}

void ui::split_panel::draw()
{
	for (auto& child : children)
	{
		child->draw();
	}
}

point ui::split_panel::perfered_size()
{
	point ret;
	if (bordered)
	{
		ret = {2, 2};
	}

	for (auto& child : children)
	{
		ret += child->perfered_size();
	}

	if (entries_seperated)
	{
		if (stacked)
		{
			ret.y += std::max((size_t)0, children.size() - 1);
		}
		else
		{
			ret.x += std::max((size_t)0, children.size() - 1);
		}
	}

	return ret;
}

point ui::split_panel::min_size()
{
	point ret;
	if (bordered)
	{
		ret = {2, 2};
	}

	for (auto& child : children)
	{
		ret += child->min_size();
	}

	if (entries_seperated)
	{
		if (stacked)
		{
			ret.y += std::max((size_t)0, children.size() - 1);
		}
		else
		{
			ret.x += std::max((size_t)0, children.size() - 1);
		}
	}

	return ret;
}

void ui::split_panel::update_sizes()
{
	auto s = size;
	sizes.clear();
	sizes.reserve(children.size());

	if (bordered)
	{
		s -= {2, 2};
	}

	if (children.empty())
	{
		return;
	}

	if (entries_seperated)
	{
		if (stacked)
		{
			s.y -= std::max((size_t)0, children.size() - 1);
		}
		else
		{
			s.x -= std::max((size_t)0, children.size() - 1);
		}
	}

	point total_min = {0, 0};
	point total_perfered_diffrence = {0, 0};
	std::vector<point> perfered_diffrence;
	perfered_diffrence.reserve(children.size());

	for (auto& child : children)
	{
		auto min = child->min_size();

		auto perfered = child->perfered_size();
		auto diff = perfered - min;

		sizes.push_back(min);
		perfered_diffrence.push_back(diff);

		total_perfered_diffrence += diff;
		total_min += min;
	}

	auto remaining_space = s - total_min;
	if (!stacked)
	{
		if (remaining_space.x > total_perfered_diffrence.x)
		{
			size_t i = 0;
			for (auto& child : children)
			{
				auto perfered = child->perfered_size();
				sizes[i].x = perfered.x;

				++i;
			}
		}
		else
		{
			size_t i = 0;
			for (auto& size2 : sizes)
			{
				size2.x += perfered_diffrence[i].x * remaining_space.x / total_perfered_diffrence.x;

				++i;
			}
		}
	}
	else
	{
		if (remaining_space.y > total_perfered_diffrence.y)
		{
			size_t i = 0;
			for (auto& child : children)
			{
				auto perfered = child->perfered_size();
				sizes[i].y = perfered.y;

				++i;
			}
		}
		else
		{
			size_t i = 0;
			for (auto& size2 : sizes)
			{
				size2.y += perfered_diffrence[i].y * remaining_space.y / total_perfered_diffrence.y;

				++i;
			}
		}
	}

	point total = {0, 0};
	for (auto& size2 : sizes)
	{
		total += size2;
	}

	remaining_space = s - total;

	if (!stacked)
	{
		if (remaining_space.x > 0)
		{
			for (auto& size2 : sizes)
			{
				size2.x = size2.x * s.x / total.x;
			}
		}
	}
	else
	{
		if (remaining_space.y > 0)
		{
			for (auto& size2 : sizes)
			{
				size2.y = size2.y * s.y / total.y;
			}
		}
	}

	total = {0, 0};
	for (auto& size2 : sizes)
	{
		total += size2;
	}

	remaining_space = s - total;

	if (!stacked)
	{
		if (remaining_space.x > 0)
		{
			sizes[0].x += remaining_space.x;
		}
	}
	else
	{
		if (remaining_space.y > 0)
		{
			sizes[0].y += remaining_space.y;
		}
	}

	if (!stacked)
	{
		for (auto& size2 : sizes)
		{
			size2.y = s.y;
		}
	}
	else
	{
		for (auto& size2 : sizes)
		{
			size2.x = s.x;
		}
	}
}

void ui::split_panel::set_size(point size, point offset)
{
	this->size = size;
	this->offset = offset;

	if (bordered)
	{
		border_direction_vector border_dir
		(
			offset.x + size.x,
			std::vector<border_direction>(offset.y + size.y, ui::border_direction::nothing)
		);
		border_color_vector border_color
		(
			offset.x + size.x,
			std::vector<nc_color>(offset.y + size.y, nc_color())
		);
		utils::mark_border(offset, size, border_dir, border_color, true, BORDER_COLOR);
		owner->border_handler->add_border_data(border_dir, border_color);
		size -= {2, 2};
		offset += {1, 1};
	}

	if (children.empty())
	{
		return;
	}

	update_sizes();

	size_t i = 0;
	for (auto& child : children)
	{
		child->set_size(sizes[i], offset);

		if (i == children.size() - 1)
		{
			continue;
		}

		border_direction_vector border_dir
		(
			offset.x + size.x,
			std::vector<border_direction>(offset.y + size.y, ui::border_direction::nothing)
		);
		border_color_vector border_color
		(
			offset.x + size.x,
			std::vector<nc_color>(offset.y + size.y, nc_color())
		);

		if (stacked)
		{
			offset.y += sizes[i].y;
			size.y -= sizes[i].y;
			if (entries_seperated)
			{
				utils::mark_line(offset, size.x, border_dir, border_color, false, BORDER_COLOR);
				offset.y += 1;
				size.y -= 1;
			}
		}
		else
		{
			offset.x += sizes[i].x;
			size.x -= sizes[i].x;
			if (entries_seperated)
			{
				utils::mark_line(offset, size.y, border_dir, border_color, true, BORDER_COLOR);
				offset.x += 1;
				size.x -= 1;
			}
		}
		if (entries_seperated)
		{
			owner->border_handler->add_border_data(border_dir, border_color);
		}
		i++;
	}
}
