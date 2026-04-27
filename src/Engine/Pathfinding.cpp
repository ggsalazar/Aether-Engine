#include "Pathfinding.h"
#include "Collision.h"
#include "Graphics/TileMap.h"
#include "Math/Math.h"
#include "../Entities/Door.h" //Entity

#include "Core.h" //Just here for debugging

void Pathfinding::Init(TileMap* t, vector<Entity*>* e) {
	tmp = t;
	entities = e;

	//Resize the grid
	grid.clear();
	grid.resize(tmp->GetMapSizeTiles().x);
	for (auto& col : grid) col.resize(tmp->GetMapSizeTiles().y);
	grid_size = Vec2{grid.size(), grid[0].size()};

	//Populate the grid
	bool node_walk = false;
	uchar node_cost = 0;
	for (int col = 0; col < grid_size.x; ++col) {
		for (int row = 0; row < grid_size.y; ++row) {
			node_walk = tmp->GetTileData({col, row}).walkable;
			node_cost = (tmp->GetTileData({col, row}).terrain == Terrain::Rough) + 1;

			grid[col][row] = { Round(col*TS + TS * .5f, row * TS + TS*.5f), false, node_walk, node_cost};
		}
	}

	/* Is this even remotely necessary anymore?
	//Once we've built out our node grid, build our network of corners
	//First, assign corners & inverse corners
	uchar adj_walkables = 0;
	for (int x = 0; x < grid_size.x; ++x) {
		for (int y = 0; y < grid_size.y; ++y) {
			//If a node is unwalkable, check its adjacent nodes to see if it is a corner: if >= 5 are walkable, that node is a corner - if exactly 1 is walkable, it's an inverse corner


			//Should "corners" on the edge of the map be counted?


			if (!grid[x][y].walkable) {
				adj_walkables = 0;
				for (char i=-1; i <= 1; ++i) {
					if (x+i < 0 or x+i >= grid_size.x) break;
					for (char j=-1; j <= 1; ++j) {
						if (y+j < 0 or y+j >= grid_size.y) break;

						adj_walkables += grid[x+i][y+j].walkable;
					}
				}
				grid[x][y].corner = adj_walkables >= 5;
				grid[x][y].inverse_corner = adj_walkables == 1;
			}
		}
	}

	//Corners assigned, build the network itself
	bool in_network = false, building_subnetwork = false;
	for (int x = 0; x < grid_size.x; ++x) {
		for (int y = 0; y < grid_size.y; ++y) {
			in_network = false;
			//For every corner, search orthogonally for other corners
			if (grid[x][y].corner) {
				//First, check to see if we're already in the corner grid
				for (int cx=0; cx < corners.size(); ++cx) {
					for (int cy=0; cy < corners[cx].size(); ++cy) {
						if (corners[cx][cy]->pos == grid[x][y].pos) {
							in_network = true;
							break;
						}
					}
					if (in_network) break;
				}
				if (in_network) continue;


				//Add this corner to the network
				//If this corner was not in the network, then we must be in a fresh sub-network
				corners.emplace_back(vector{&grid[x][y]});
				Vec2i true_xy = {x, y};
				building_subnetwork = true;
				uchar search_dir = 0; //0 = up, 1 = right, 2 = down, 3 = left

				while (building_subnetwork) {
					switch (search_dir) {
						default:
							//This should never happen, but best to have it here jic
							building_subnetwork = false;
						break;

						//Search up
						case 0:
							for (int i=y-1; i > 0; --i) {
								if (grid[x][i].corner) {
									//Check if this corner is in this sub-network; if it is, we've hit the starting corner and can stop building
									for (auto& c : corners[corners.size()-1]) {
										if (c->pos == grid[x][i].pos) {
											building_subnetwork = false;
											break;
										}
									}

									if (building_subnetwork) {
										corners[corners.size()-1].emplace_back(&grid[x][i]);

										//Continue the search from this point
										//Going to search right, so set y to i
										++search_dir;
										y = i;
										break;
									}
								}
								//Hit the edge of the map without finding a corner, time to search right
								search_dir += i==1;
							}
						break;

						//Search right
						case 1:
							for (int i=x+1; i < grid_size.x-1; ++i) {
								if (grid[i][y].corner) {
									for (auto& c : corners[corners.size()-1]) {
										if (c->pos == grid[i][y].pos) {
											building_subnetwork = false;
											break;
										}
									}

									if (building_subnetwork) {
										corners[corners.size()-1].emplace_back(&grid[i][y]);

										//Going to search down, so set x to i
										++search_dir;
										x = i;
										break;
									}
								}
								search_dir += i==grid_size.x-2;
							}
						break;

						//Search down
						case 2:
							for (int i=y+1; i < grid_size.y-1; ++i) {
								if (grid[x][i].corner) {
									for (auto& c : corners[corners.size()-1]) {
										if (c->pos == grid[x][i].pos) {
											building_subnetwork = false;
											break;
										}
									}

									if (building_subnetwork) {
										corners[corners.size()-1].emplace_back(&grid[x][i]);

										//Going to search left, so set y to i
										++search_dir;
										y = i;
										break;
									}
								}
								search_dir += i==grid_size.y-2;
							}
						break;

						//Search Left
						case 3:
							for (int i=x-1; i > 0; --i) {
								if (grid[i][y].corner) {
									for (auto& c : corners[corners.size()-1]) {
										if (c->pos == grid[i][y].pos) {
											building_subnetwork = false;
											break;
										}
									}

									if (building_subnetwork) {
										corners[corners.size()-1].emplace_back(&grid[i][y]);

										//Search up, set x to i
										search_dir = 0;
										x = i;
										break;
									}
								}
								search_dir += i==1;
							}
						break;
					}
				}
				//Reset x & y so the search can properly resume
				x = true_xy.x, y = true_xy.y;
			}
		}
	}

	cout << "Corner sub-networks (3): " << corners.size() << endl;
	*/
}

void Pathfinding::UpdateNodeGrid() {
	bool node_occ = false;

	for (int col = 0; col < grid_size.x; ++col) {
		for (int row = 0; row < grid_size.y; ++row) {
			//For now, we're going to say a node is not walkable if an entity is standing on that tile
			// In the future, this should be *way* more nuanced (i.e. creature vs object, friend or foe, size of entity, etc)
			if (tmp->GetTileData({ col, row }).walkable) {
				for (const auto& e : *entities) {
					if (dynamic_cast<Door*>(e)) continue;

					node_occ = Collision::RectPoint(Rect(Vec2{ col * TS, row * TS }, TS), e->GetPos());

					if (node_occ) break;
				}

				grid[col][row].occupied = node_occ;
				//Reset node A* values
				grid[col][row].g = 0;
				grid[col][row].h = 0;
				grid[col][row].f = 0;
				grid[col][row].parent = nullptr;
			}
		}
	}
}

vector<Vec2i> Pathfinding::FindPath(const Vec2i& start, Vec2i& goal, Entity* target) {
	//GameMaster::DefaultAction() checks if we are close enough to our goal

	//Get the grid coords closest to the start and goal
	Vec2i grid_start = Round((start.x - TS * .5f) / TS, (start.y - TS * .5f) / TS),
		  grid_goal = Round((goal.x - TS * .5f) / TS, (goal.y - TS * .5f) / TS);
	Math::Clamp(grid_goal.x, 0, grid_size.x-1); Math::Clamp(grid_goal.y, 0, grid_size.y-1);

	//If the goal node is unwalkable return {}
	if (!grid[grid_goal.x][grid_goal.y].walkable) return {};

	//Select the closest unoccupied and unclaimed node to the start node
	if (target) {
		Vec2i closest_grid_goal = grid_goal;
		int i = 0, j = 0;
		Node* n = nullptr;

		for (i = grid_goal.x - 1; i <= grid_goal.x + 1; ++i) {
			if (i < 0 or grid_size.x <= i) continue;
			for (j = grid_goal.y - 1; j <= grid_goal.y + 1; ++j) {
				if (j < 0 or grid_size.y <= j) continue;

				n = &grid[i][j];
				//If n exists, is walkable, is unclaimed, and is closer to the start than the current closest goal, set the closest goal to n's grid coordinates
				//This will need to be modified to test if n is closer to the second-to-last node than the current closest goal
				if (n and n->walkable and !n->claimed and Distance(n->pos, grid[grid_start.x][grid_start.y].pos) < Distance(grid[closest_grid_goal.x][closest_grid_goal.y].pos, grid[grid_start.x][grid_start.y].pos))
					closest_grid_goal = { i, j };
			}
		}

		//If the closest node we found was the goal node, that means we didn't find a closer tile to stand on,
		// which means we have no path to the target
		if (closest_grid_goal == grid_goal) return {};
		grid_goal = closest_grid_goal;
	}

	UpdateNodeGrid();

	//Adjust the world position of the goal
	goal = grid[grid_goal.x][grid_goal.y].pos;

	//A*
	//Create our lists
	priority_queue<Node*, vector<Node*>, Node::Compare> open_list;
	unordered_map<Node*, bool> closed_list;

	//Initialize start node
	Node* start_node = &grid[grid_start.x][grid_start.y];
	start_node->g = start_node->f = start_node->h = 0;
	open_list.push(start_node);

	//Define possible movements: N/NE/E/SE/S/SW/W/NW
	const Vec2i directions[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

	//Find the path
	Node* current;
	Node* neighbor;
	Vec2i neighbor_pos;
	while (open_list.size()) {
		current = open_list.top();
		open_list.pop();

		if (closed_list[current]) continue;

		//Check each direction around us
		for (const auto& dir : directions) {
			neighbor_pos = current->pos + dir * TS;

			//Out of bounds
			if (neighbor_pos.x <= 0 || neighbor_pos.y <= 0 || neighbor_pos.x >= tmp->GetMapSizePixels().x || neighbor_pos.y >= tmp->GetMapSizePixels().y)
				continue;

			neighbor = &grid[neighbor_pos.x/TS][neighbor_pos.y/TS];
			//Unwalkable or already processed
			if (!neighbor->walkable or closed_list[neighbor])
				continue;

			//Is this neighbor the goal?
			if (neighbor->pos == goal) {

				//Set the parent and change current
				neighbor->parent = current;
				current = neighbor;

				//Build & return the path
				vector<Vec2i> path;
				while (current->parent) {
					path.push_back(current->pos);
					current = current->parent;
				}

				reverse(path.begin(), path.end());
				path = SmoothPath(path);
				return path;
			}

			//Set the parent
			neighbor->parent = current;

			//Calculate neighbor's g, h, and f; only work on unprocessed nodes (priority_queue throws a bitch fit otherwise)
			if (neighbor->g == 0) {
				float move_cost = !dir.x or !dir.y ? 1.f : sqrt2;
				neighbor->g = current->g + move_cost + neighbor->cost;
				neighbor->h = Heuristic(neighbor->pos, goal);
				neighbor->f = neighbor->g + neighbor->h;
				//Add this bitch to the open list (automatically sorted by f values)
				open_list.push(neighbor);
			}
		}

		//Add the current node to the closed list
		closed_list[current] = true;
	}


	return {};
}

bool Pathfinding::LineOfSight(const Vec2i& from, const Vec2i& to) const {
	//Bresenham�s Line Algorithm or DDA
	//Return false if any tile along the line is unwalkable

	Vec2i delta = to - from;
	int steps = max(abs(delta.x), abs(delta.y)) / TS;
	Vec2f dir = Vec2f(delta.x, delta.y) / (float)steps;

	Vec2i point;
	for (int i = 1; i < steps; ++i) {
		point = Vec2{ (from.x + dir.x * i) / TS, (from.y + dir.y * i) / TS };
		if (!grid[point.x][point.y].walkable)
			return false;
	}
	return true;
}

vector<Vec2i> Pathfinding::SmoothPath(const vector<Vec2i>& raw) const {
	if (raw.size() == 1) return { Vec2i({ raw[0].x, raw[0].y }) };

	vector<Vec2i> result;
	Vec2i anchor = raw[0], last_visible = raw[1];
	result.push_back(anchor);

	for (size_t k = 2; k < raw.size(); ++k) {
		if (LineOfSight(anchor, raw[k])) last_visible = raw[k];
		else {
			//Check to see if the next point is visible from any adjacent tiles; if it is, add that tile to the result and go from there
			Vec2i next_closest = last_visible;
			for (char i = -1; i <= 1; ++i) {
				for (char j = -1; j <= 1; ++j) {
					if (grid[last_visible.x/TS + i][last_visible.y/TS + j].walkable and Distance(grid[last_visible.x/TS + i][last_visible.y/TS + j].pos, raw[k]) < Distance(last_visible, raw[k]))
						next_closest = grid[last_visible.x/TS + i][last_visible.y/TS + j].pos;
				}
			}
			result.push_back(last_visible);
			result.push_back(next_closest);
			anchor = next_closest;
			last_visible = next_closest;
		}
	}

	result.push_back(raw.back());
	return result;

	//Build paths between each point in the smoothed path
	/*
	vector<Vec2i> line, final;
	Vec2i pos1, pos2, diff, sign, err;
	for (i = 0; i < smoothed.size()-1; ++i) {
		line.clear();
		pos1 = smoothed[i],	pos2 = smoothed[i+1];
		diff = {abs(pos2.x - pos1.x), abs(pos2.y - pos1.y)};
		err.x = diff.x - diff.y;
		sign.x = pos1.x < pos2.x ? TS : -TS;
		sign.y = pos1.y < pos2.y ? TS : -TS;
		line.push_back(pos1);

		while (pos1.x != pos2.x or pos1.y != pos2.y) {
			err.y = 2 * err.x;
			if (err.y > -diff.y) {
				err.x -= diff.y;
				pos1.x += sign.x;
			}
			if (err.y < diff.x) {
				err.x += diff.x;
				pos1.y += sign.y;
			}
			line.push_back(pos1);
		}

		//Avoid duplicate points
		if (final.size())
			line.erase(line.begin());

		final.insert(final.end(), line.begin(), line.end());
	}
	return final;
	*/
}

float Pathfinding::Heuristic(const Vec2i& a, const Vec2i& b) {
	Vec2i d = {abs(a.x - b.x), abs(a.y - b.y)};
	return (d.x + d.y) + (sqrt2 - 2) * std::min(d.x, d.y);
}