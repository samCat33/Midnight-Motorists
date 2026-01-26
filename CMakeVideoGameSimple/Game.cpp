#include "Game.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int Game::numFrames = 0;
int frameTracker = 0;
int lapFrame;
int lapFrameTracker;

SDL_Rect carSrcRect = {0, 0, 13, 10};
SDL_Rect carDestRect = { 200, 200, 100, 100 };

Manager manager;
Entity& player(manager.addEntity());
Entity& wasd(manager.addEntity());
Entity& bigText(manager.addEntity());
SpriteComponent* wasdSprite = nullptr;
SpriteComponent* bigTextSprite = nullptr;

bool canHitEnemy = true;
bool enemyCanSpawn = true;
bool lineCanSpawn = false;
bool timeToAddDigit = false;
bool timeToAddMPHDigit = false;
bool needToAddLapDigit = false;
bool wasdShowing = true;
bool topSpeed = false;
bool countdown = true;
bool showLapBigText = false;
bool changedBigText = false;

int enemySpawnX;
int enemySpawnY;

int lives = STARTING_LIVES;
int firstDigitX;
int mphDigitX;
int lapDigitX;
int wasdWidth;
int wasdHeight;
int digitY;
int numDigits = 0;
int numMPHDigits = 0;

double speedModifier = 0;
double totalSpeed = 0;

int enemySpawnTime = 0;
int lineSpawnTime = 0;
int speedResetTime = 0;
int enemyHitTime = 0;
int lastResetTime = 0;
int gameStartTime = 0;

//All the possible group labels of objects
enum groupLabels : size_t {
	PLAYERS_GROUP,
	ENEMIES_GROUP,
	FINISH_LINE,
	TOP_LINE_GROUP,
	BOTTOM_LINE_GROUP,
	BARRIER_GROUP,
	DIGITS_GROUP,
	MPH_DIGITS_GROUP,
	LAP_DIGITS_GROUP,
	LIVES_GROUP,
	MAP_STUFF,
	ATARI_LINES
};

//A vector of all the collider of game objects
//Used for collision logic
vector<ColliderComponent*> Game::colliders;

//Get all the entities in groups
auto& enemies(manager.GetEntitiesInGroup(ENEMIES_GROUP));
auto& finishLine(manager.GetEntitiesInGroup(FINISH_LINE));
auto& topLines(manager.GetEntitiesInGroup(TOP_LINE_GROUP));
auto& bottomLines(manager.GetEntitiesInGroup(BOTTOM_LINE_GROUP));
auto& barriers(manager.GetEntitiesInGroup(BARRIER_GROUP));
auto& digits(manager.GetEntitiesInGroup(DIGITS_GROUP));
auto& mphDigits(manager.GetEntitiesInGroup(MPH_DIGITS_GROUP));
auto& lapDigits(manager.GetEntitiesInGroup(LAP_DIGITS_GROUP));
auto& lifeIcons(manager.GetEntitiesInGroup(LIVES_GROUP));
auto& mapStuff(manager.GetEntitiesInGroup(MAP_STUFF));
auto& atariLines(manager.GetEntitiesInGroup(ATARI_LINES));

//Used for digits
int* iterations = nullptr;
int* mphIterations = nullptr;
int* lapIterations = nullptr;

//Used for spawning new lines
double* topLeftLineX = nullptr;
double* finishLineX = nullptr;

//Add the edges of the road
void AddBarrier(int x, int y) {

	Entity& barrier(manager.addEntity());
	barrier.addComponent<TransformComponent>(x,
		y, WINDOW_H * 0.05,
		WINDOW_W * 1.8, 1, 0);
	barrier.addComponent<SpriteComponent>(EDGE_PATH);
	barrier.addGroup(BARRIER_GROUP);
	barrier.getComponent<TransformComponent>().velocity +=
		obstacleVector;

}

//Add lines for the road
void AddLine (int x, int y, groupLabels g) {
	Entity& line(manager.addEntity());
	line.addComponent<TransformComponent>(x, y,
		WINDOW_H * 0.05, WINDOW_W * 0.15, 1, STARTING_SPEED);
	line.addComponent<SpriteComponent>(LINE_PATH);
	line.addGroup(g);
	line.getComponent<TransformComponent>().velocity +=
		obstacleVector;
}

//Add finish line to lap
void AddFinishLine(int x) {
	Entity& finishLine(manager.addEntity());
	finishLine.addComponent<TransformComponent>(x, 0, 
	WINDOW_H, WINDOW_W * 0.1, 1.0, STARTING_SPEED,
	FINISH_LINE_TAG);
	finishLine.addComponent<SpriteComponent>(FINISH_LINE_PATH);
	finishLine.addGroup(FINISH_LINE);
	finishLine.addComponent<ColliderComponent>(WINDOW_H, WINDOW_W * 0.1, false);
	finishLine.getComponent<TransformComponent>().velocity +=
		obstacleVector;
}

//Add digits
void AddDigit(int x, int y, double fps, groupLabels g, int iterations = 0) {
	Entity& digit(manager.addEntity());
	digit.addComponent<TransformComponent>(x, y, WINDOW_H * 0.1,
	WINDOW_W * 0.03, 1, 0);
	digit.addComponent<SpriteComponent>(FONT_PATH, 0, 10, fps, 24);
	digit.getComponent<SpriteComponent>().customIterations = iterations;
	digit.addGroup(g);
}


void ResetMPH() {
	for (auto& m : mphDigits) {
		m->destroy();
	}
	mphDigits.clear();
	AddDigit(mphDigitX, WINDOW_H * 0.08, MPH_DIGIT_FPS, MPH_DIGITS_GROUP, 40);
	AddDigit(mphDigitX - (WINDOW_W * 0.03), WINDOW_H * 0.08, 
	MPH_DIGIT_FPS / 10.0, MPH_DIGITS_GROUP, 4);
	numMPHDigits = 2;
	
	mphDigits.at(0)->getComponent<SpriteComponent>().resetTime
	= currentTime;
	mphDigits.at(1)->getComponent<SpriteComponent>().resetTime 
	= currentTime;

	mphIterations = &(mphDigits.at(0)->getComponent<SpriteComponent>()
		.iterations);

	topSpeed = false;
}

//Add Lap and MPH text
void AddLapAndMPH() {
	Entity& lap(manager.addEntity());
	lap.addComponent<TransformComponent>(WINDOW_W * 0.05, WINDOW_H * 0.08,
	WINDOW_H * 0.1, WINDOW_W * 0.1, 1, 0);
	lap.addComponent<SpriteComponent>(LAP_PATH);
	lap.addGroup(MAP_STUFF);

	Entity& mph(manager.addEntity());
	mph.addComponent<TransformComponent>(WINDOW_W * 0.4, WINDOW_H * 0.08,
		WINDOW_H * 0.1, WINDOW_W * 0.1, 1, 0);
	mph.addComponent<SpriteComponent>(MPH_PATH);
	mph.addGroup(MAP_STUFF);
}

void AddLives() {
	//Add Transform and Sprite Components for life icons
	for (int i = 0; i < STARTING_LIVES; ++i) {
		Entity& life(manager.addEntity());
		life.addComponent<TransformComponent>(
		WINDOW_W * 0.05 + (WINDOW_W * 0.1 * i), WINDOW_H * 0.9,
		WINDOW_H * 0.08, WINDOW_W * 0.08);
		life.addComponent<SpriteComponent>(PLAYER_PATH);
		life.addGroup(LIVES_GROUP);
	}
}

void DestroyLife() {
	lifeIcons.at(lifeIcons.size() - 1)->destroy();
	
	//One life is lost
	lives--;
}

void ShowWASD() {
	wasd.addComponent<TransformComponent>(
		WINDOW_W * 0.85, WINDOW_H * 0.7,
		wasdHeight, wasdWidth
	);
	wasd.addComponent<SpriteComponent>(WASD_PATH);
	wasd.addGroup(MAP_STUFF);
	wasdSprite = &(wasd.getComponent<SpriteComponent>());
}

void AddBigText() {
	bigText.addComponent<TransformComponent>(
		WINDOW_W * 0.35, WINDOW_H * 0.2,
		WINDOW_H * 0.6, WINDOW_W * 0.4
	);
	bigText.addComponent<SpriteComponent>(BIG_TEXT_PATH,0,5,0, 
	BIG_TEXT_SHIFT);
	bigText.addGroup(MAP_STUFF);
	bigTextSprite = &(bigText.getComponent<SpriteComponent>());
	bigTextSprite->test = false;
}

void AddStats() {
	//Add Lap and MPH text
	AddLapAndMPH();

	//Add starting digits for score, lap, and mph
	AddDigit(firstDigitX, digitY, FIRST_DIGIT_FPS, DIGITS_GROUP);
	++numDigits;
	AddDigit(lapDigitX, digitY, 0, LAP_DIGITS_GROUP, 1);
	ResetMPH();

	//Get iterations of each starting digit
	iterations = &(digits.at(0)->getComponent<SpriteComponent>()
		.iterations);
	mphIterations = &(mphDigits.at(0)->getComponent<SpriteComponent>()
		.iterations);
	lapIterations = &(lapDigits.at(0)->getComponent<SpriteComponent>()
		.customIterations);
	*lapIterations = 1;
}

void ChangeBigText() {
	bigTextSprite->customIterations = 0;
	bigTextSprite->changeSrcRectW(384);
	bigTextSprite->changeSrcRectX(900);
	bigTextSprite->hide();
	changedBigText = false;
}

void AddAtariLines() {
	for (int i = 0; i < 40; ++i) {
		Entity& atariLine(manager.addEntity());
		atariLine.addComponent<TransformComponent>(
			0, WINDOW_H * 0.01 + (WINDOW_H * 0.025 * i),
			WINDOW_H * 0.005, WINDOW_W
		);
		atariLine.addComponent<SpriteComponent>(
		ATARI_LINE_PATH);
		atariLine.addGroup(ATARI_LINES);
	}
}

void draw(bool customSpeed, Entity* b) {
	if (customSpeed) {
		TransformComponent& t = b->getComponent<TransformComponent>();
		t.speed = t.startSpeed + speedModifier;
	}
	b->draw();
}

//Core of game logic
void Game::runGame() {
	currentTime = SDL_GetTicks();
	++numFrames;
	
	handleEvents();
	update();
	render();

	frameTime = SDL_GetTicks() - currentTime;

	if (frameTime < DELTA_TIME) {
		SDL_Delay(DELTA_TIME - frameTime);
	}

}

//Runs when game starts 
void Game::init() {
	firstDigitX = WINDOW_W * 0.85;
	mphDigitX = WINDOW_W * 0.6;
	lapDigitX = WINDOW_W * 0.25;
	digitY = WINDOW_H * 0.08;
	wasdWidth =  WINDOW_W * 0.25;
	wasdHeight =  WINDOW_H * 0.25;

	//Add player
	player.addComponent<TransformComponent>
	(200, 200, PLAYER_H, PLAYER_W, 1, PLAYER_SPEED, PLAYER_TAG);
	player.addComponent<SpriteComponent>(PLAYER_PATH);
	player.addComponent<ColliderComponent>
	(PLAYER_HITBOX_H, PLAYER_HITBOX_W);
	player.addComponent<KeyboardController>();
	player.addGroup(PLAYERS_GROUP);
	isRunning = true;

	//Add road edges and median
	AddBarrier(0, 0);
	AddBarrier(0, (WINDOW_H / 2) -
		(WINDOW_H * 0.05));
	AddBarrier(0, (WINDOW_H)
		-(WINDOW_H * 0.05));

	//Add Top Lines
	AddLine(0, WINDOW_H * 0.2, TOP_LINE_GROUP);
	AddLine(WINDOW_W * 0.25, WINDOW_H * 0.2, TOP_LINE_GROUP);
	AddLine(WINDOW_W * 0.5, WINDOW_H * 0.2, TOP_LINE_GROUP);
	AddLine(WINDOW_W * 0.75, WINDOW_H * 0.2, TOP_LINE_GROUP);
	AddLine(WINDOW_W, WINDOW_H * 0.2, TOP_LINE_GROUP);

	//Add Bottom Lines
	AddLine(0, WINDOW_H * 0.7, BOTTOM_LINE_GROUP);
	AddLine(WINDOW_W * 0.25, WINDOW_H * 0.7, BOTTOM_LINE_GROUP);
	AddLine(WINDOW_W * 0.5, WINDOW_H * 0.7, BOTTOM_LINE_GROUP);
	AddLine(WINDOW_W * 0.75, WINDOW_H * 0.7, BOTTOM_LINE_GROUP);
	AddLine(WINDOW_W, WINDOW_H * 0.7, BOTTOM_LINE_GROUP);

	//lapDigits.at(0)->getComponent<SpriteComponent>().testDigit = true;

	topLeftLineX = &(topLines.at(0)
		->getComponent<TransformComponent>().position.x);

	AddLives();
	ShowWASD();
	AddBigText();
	AddAtariLines();
}

//Handles the events of the game
void Game::handleEvents() {
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

//Runs every frame
void Game::update(){

	//currentTime is a double
	currentTime = SDL_GetTicks();

	//Check to see if any entities are due for destruction
	manager.refresh();

	//Update all entities
	manager.update();

	//Used to flash WASD on screen
	if (wasdShowing) {
		if (numFrames % 20 == 0) {
			++frameTracker;
		}
		if (frameTracker % 2 == 0) {
			wasdSprite->show();
		}
		else {
			wasdSprite->hide();
		}
		if (numFrames > 219) {
			wasdShowing = !wasdShowing;
		}
	}

	//Hide WASD flashing when done
	if (!wasdShowing) {
		wasdSprite->hide();
	}

	//Hide GO when done
	if (numFrames > 320) {
		if (!changedBigText) {
			ChangeBigText();
		}
	}

	//Do countdown timer
	if (countdown) {
		//Set speed modifier to prevent obstacles from moving
		speedModifier = -3;

		//Iterate through 3, 2, 1, GO
		if (numFrames < 239) {
			bigTextSprite->customIterations = numFrames / 60;
		}
		//Add stats and stop countdown when countdown is done
		else {
			AddStats();
			gameStartTime = currentTime;
			speedModifier = 0;
			countdown = !countdown;
		}
	}

	else{
		//Used for making objects move as player speeds up
		totalSpeed += STARTING_SPEED + speedModifier;

		//Get parts of player's collider component
		Vector2D currPosition = player.getComponent<TransformComponent>().position;
		int playerTop = player.getComponent<ColliderComponent>().topEdge;
		int playerLeft = player.getComponent<ColliderComponent>().leftEdge;
		int playerRight = player.getComponent<ColliderComponent>().rightEdge;
		int playerBottom = player.getComponent<ColliderComponent>().bottomEdge;

		//For each collider in the vector of all colliders
		for (auto cc : colliders) {

			//If AABB occurs between the player's collider component and the 
			//given collider and the collider's tag is not the player's tag
			//(in order to prevent the player from collidng with themselves)
			if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)
				&& cc->transform->tag != PLAYER_TAG) {

				//Get the edges of the collider component
				int ccLeft = cc->leftEdge;
				int ccRight = cc->rightEdge;
				int ccTop = cc->topEdge;
				int ccBottom = cc->bottomEdge;

				//There must be overlap on all sides as defined
				//in order for a proper collision to have occurred.
				int overlapRight = playerRight - ccLeft; // Player hits left edge
				int overlapLeft = -playerLeft + ccRight; // Player hits right edge
				int overlapTop = -playerTop + ccBottom; //Player hits bottom edge
				int overlapBottom = playerBottom - ccTop; //Player hits top edge
				bool collision = (overlapRight > 0 && overlapLeft > 0 
				&& overlapTop > 0 && overlapBottom > 0);

				if (collision) {
					//Get the smallest overlap
					int minOverlap = min({overlapRight, overlapLeft, 
					overlapTop, overlapBottom});

					//Correct player's position using a resolve velocity vector
					// based on which overlap is the smallest
					Vector2D resolve;
					resolve.Zero();

					/* Only move around collider component if the player should
					not pass through*/
					if (cc->hittable) {
						//Using the smallest overlap prevents more correction
						// than necessary
						if (minOverlap == overlapRight) {
							resolve = Vector2D(-overlapRight, 0);
						}
						else if (minOverlap == overlapLeft) {
							resolve = Vector2D(overlapLeft, 0);
						}
						else if (minOverlap == overlapTop) {
							resolve = Vector2D(0, overlapTop);
						}
						else if (minOverlap == overlapBottom) {
							resolve = Vector2D(0, -overlapBottom);
						}
					}

					//When player hits enemy
					if (cc->transform->tag == ENEMY_TAG) {
						if (canHitEnemy) {
							//Prevent instant recollision
							canHitEnemy = false;

							//Speed is reset, and timers are set
							speedModifier = 0;
							speedResetTime = currentTime;
							enemyHitTime = currentTime;

							//If this is the first crash, reset 
							//game start timer
							gameStartTime = (gameStartTime != 0) ? 0 : 0;

							//Player loses one life
							DestroyLife();

							//Kills the player if that was their last life
							if (lives < 0) {
								playerIsAlive = false;
							}
							//Ensures player gets pushed far enough away from enemy
							resolve *= 12;

							//Reset the MPH the player is going
							ResetMPH();
						}
						else {
							resolve = resolve.Zero();
						}
					}

					//When player hits finish line and completes a lap
					if (cc->transform->tag == FINISH_LINE_TAG && !needToAddLapDigit
					&& cc->collidable) {
						
						//Add 1 to lap /*MOVE THIS BACK*/
						lapDigits.at(0)->getComponent<SpriteComponent>().
							customIterations++;

						//If a multiple of 10 is reached
						if ((*lapIterations % 10) == 0) {

							//Change each place value digit 
							//(from tens place upward)
							if (lapDigits.size() > 1) {
								for (int i = 1; i < lapDigits.size(); ++i) {
									lapDigits.at(i)->getComponent<SpriteComponent>()
										.customIterations =
										static_cast<int>(*lapIterations / pow(10, i));
								}
							}

							//If a new place value is reached, add a digit
							if (log10(*lapIterations) ==
								static_cast<int>(log10(*lapIterations))) {
								AddDigit(lapDigitX - (WINDOW_W * 0.03 * lapDigits.size()),
									digitY, 0, LAP_DIGITS_GROUP, 1);
							}
						}

						//Flash LAP on screen
						showLapBigText = true;
						lapFrame = numFrames;
						lapFrameTracker = 0;

						cout << "Lap!" << endl;

						//cc->collidable = false prevents lap from incrementing
						//if the player gets hit back onto finish line
						cc->collidable = false;
						needToAddLapDigit = true;
					}


					//Move player by resolve vector and reset velocity
					currPosition += resolve;
					player.getComponent<TransformComponent>().velocity
					= Vector2D(0, 0);
				}
			}
		}
		//End of for loop

		/*For collisions against the edges*/

		Vector2D moveVec(0, 0);
	
		//Top Edge
		if (playerTop < 0) {
			moveVec.y += 0 - playerTop;
		}
		//Bottom Edge
		if (playerBottom > WINDOW_H) {
			moveVec.y -= playerBottom - WINDOW_H;
		}
		//Left Edge
		if (playerLeft < 0) {
			moveVec.x += 0 - playerLeft;
		}
		//Right Edge
		if (playerRight > WINDOW_W) {
			moveVec.x -= playerRight - WINDOW_W;
		}
	
		//Move player off the edges
		currPosition += moveVec;
		player.getComponent<TransformComponent>().position = currPosition;
	

		//ALL LOGIC FOR WHEN PLAYER IS STILL ALIVE
		if (playerIsAlive) {

			topLeftLineX = &(topLines.at(0)->
				getComponent<TransformComponent>().position.x);

			//SPEED MODIFIER
			if (speedModifier < 15.0) {
				speedModifier = (currentTime - speedResetTime - gameStartTime) / 1000.0;
			}

			//If an enemy can spawn, spawn the enemy
			if (enemyCanSpawn) {
				//Add enemy entity and set position and speed
				auto& enemy(manager.addEntity());
				enemySpawnX = WINDOW_W + (WINDOW_W * 0.3);
				enemySpawnY = WINDOW_H * Y_SPAWNS.at(rand() % Y_SPAWNS.size());
				int enemySpeed = rand() % ENEMY_SPEED_RANGE + 3;


				//Add a transform component that holds position and speed values
				enemy.addComponent<TransformComponent>
				(enemySpawnX, enemySpawnY, ENEMY_H, ENEMY_W, 
				1, enemySpeed, ENEMY_TAG);
			
				//Add sprite and collider components
				enemy.addComponent<SpriteComponent>(ENEMY_PATH, 180);
				enemy.addComponent<ColliderComponent>
				(ENEMY_HITBOX_H, ENEMY_HITBOX_W, false);
			
				//Add to group, add velocity, and set time
				enemy.addGroup(ENEMIES_GROUP);
				enemy.getComponent<TransformComponent>().velocity += 
				obstacleVector;
				enemySpawnTime = currentTime;

				//Reduce enemy spawn time for next spawn
				if (ENEMY_SPAWNRATE_MS >= 1500) {
					ENEMY_SPAWNRATE_MS -= 100;
				}
				enemyCanSpawn = false;
			}

			//SPAWNING LANES	
			if (*topLeftLineX < (WINDOW_W * -0.25)) {
				lineCanSpawn = true;
			}

			if (lineCanSpawn) {
				AddLine(WINDOW_W, WINDOW_H * 0.2, TOP_LINE_GROUP);
				AddLine(WINDOW_W, WINDOW_H * 0.7, BOTTOM_LINE_GROUP);
				lineSpawnTime = currentTime;
				topLines.at(0)->destroy();
				bottomLines.at(0)->destroy();

				lineCanSpawn = false;
			}

			//Enemies can spawn whenever the time since the last enemy
			//spawn is > ENEMY_SPAWNRATE_MS (see GameVariables.cpp)
			if (currentTime - enemySpawnTime > ENEMY_SPAWNRATE_MS) {
				enemyCanSpawn = true;
			}

			if (currentTime - enemyHitTime > ENEMY_HIT_COOLDOWN) {
				canHitEnemy = true;
			}

			//ADDING DIGITS TO CURRENT SCORE

			//Everytime the number of iterations reaches a new place value, 
			//it is time to add a new score digit
			if ( (  (*iterations) / (static_cast<int>(pow(10, numDigits))  ) > 0)) {
				timeToAddDigit = true;
			}

			//Add a new score digit
			if (timeToAddDigit) {
				AddDigit(firstDigitX - (WINDOW_W * 0.03 * numDigits),
					digitY,
					FIRST_DIGIT_FPS / (pow(10, numDigits)), DIGITS_GROUP);

				++numDigits;
				timeToAddDigit = false;
			}

			//Everytime mph reaches a new place value
			if (((*mphIterations) / (static_cast<int>(pow(10, numMPHDigits))) > 0)) {
				timeToAddMPHDigit = true;
			}

			//Add a new mph digit
			if (timeToAddMPHDigit) {
				AddDigit(mphDigitX - (WINDOW_W * 0.03 * numMPHDigits),
					digitY,
					MPH_DIGIT_FPS / (pow(10, numMPHDigits)), MPH_DIGITS_GROUP
					,1);
				++numMPHDigits;

				//This is necessary to ensure it always starts at 1
				// (and the amount of time passed since game began
				// does not affect its value)
				mphDigits.at(numMPHDigits - 1)->getComponent
				<SpriteComponent>().resetTime = currentTime;
			
				timeToAddMPHDigit = false;
			}

			if (showLapBigText) {
				if ((numFrames - lapFrame) % 3 == 0) {
					++lapFrameTracker;
				}
				if (lapFrameTracker % 2 == 0) {
					bigTextSprite->show();
				}
				else {
					bigTextSprite->hide();
				}
				if ((numFrames - lapFrame) > 79) {
					showLapBigText = !showLapBigText;
				}

			}

			if (finishLineX != nullptr) {
				if (*finishLineX < 
				player.getComponent<TransformComponent>().position.x) {
					needToAddLapDigit = false;
				}
			}

			//Add a finish line after the totalSpeed variable exceeds
			// a certain value
			if (totalSpeed > 8000) {
				if (finishLineX != nullptr) {
					finishLine.at(0)->destroy();
				}
				AddFinishLine(WINDOW_W);
				finishLineX = &(finishLine.at(0)->getComponent
				<TransformComponent>().position.x);
				totalSpeed = 0;
			}

			//Stop increasing mph value when top speed is reached
			if (speedModifier >= 15) {
				for (auto& m : mphDigits) {
					m->getComponent<SpriteComponent>().iterating = false;
				}
			}
		}
	}
}


//Also runs every frame, handles drawing of objects
void Game::render() {

	//Set render draw color and clear renderer
	SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);

	//Clear our SDL renderer
	//IMPORTANT: THIS CLEARS THE RENDERER BY FILLING IT WITH
	// THE PREVIOUSLY SET DRAW COLOR, SO CALL THIS AFTER
	// SDL_SetRenderDrawColor()!!
	SDL_RenderClear(RENDERER);

	for (auto& b : barriers) {
		draw(false, b);
	}
	for (auto &l : topLines) {
		draw(true, l);
	}
	for (auto& l : bottomLines) {
		draw(true, l);
	}
	for (auto& f : finishLine) {
		draw(true, f);
	}
	for (auto& d : digits) {
		draw(false, d);
	}
	for (auto& m : mphDigits) {
		draw(false, m);
	}
	for (auto& l : lapDigits) {
		draw(false, l);
	}
	for (auto& l : lifeIcons) {
		draw(false, l);
	}
	for (auto& s : mapStuff) {
		draw(false, s);
	}
	//Draw the player
	player.draw();
	for (auto& a : atariLines) {
		draw(false, a);
	}
	//Draw each enemy
	for (auto& e : enemies) {

		if (e->getComponent<TransformComponent>().position.x > -200) {
			draw(true, e);
		}
		else {
			e->destroy();
		}

	}
	
	//Present the renderer
	SDL_RenderPresent(RENDERER);
}

//Runs when game is closed
void Game::clean(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(RENDERER);
	SDL_Quit();
	cout << "Game was cleaned!" << endl;
}