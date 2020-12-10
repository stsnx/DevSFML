#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include <sstream>
#include <fstream>
#include <string>
#include<vector>
#include<Windows.h>
#include "Ship.h"
#include "Platform.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Skill.h"
#include "EnemyBullet.h"
#include "Eliminate.h"
#include "Boss.h"
#include "textbox.h"
#include <algorithm>
using namespace sf;
using namespace std;
template <typename T>
string str(const T& x)
{
	ostringstream oss;
	oss << x;
	return oss.str();
}
static const float	VIEW_IN = 512.0F;
void ReSizeView(const RenderWindow& window, View& view)
{
	float ratio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(ratio * VIEW_IN, VIEW_IN);
}
int randomRange(int l, int r)
{
	int t = rand() % r- rand() % r;
	while (t<l || t>r) t = rand() % r - rand() % r;
	return t;
}
// vector in
vector<Bullet> bullets;
vector<Platform> platforms;
vector<Platform> Levels;
vector<Platform> interface_ShipHealth;
vector<Platform> interface_ShipMana;
vector<Platform> interface_ShipSkill;
vector<Platform> interface_Shipshield;
vector<Platform> interface_Shipenchant;
vector<Platform> interface_ShipshieldBar;
vector<Platform> interface_Boosts;
vector<EnemyBullet> enemybullets;
vector<Enemy> enemys;
vector<Boss> bosss;
vector<Skill> skills;
vector<Skill> shields;
vector<Ship> boosts;
vector<Eliminate> players;
vector<Eliminate> charging;
vector<Eliminate> shieldGone_animated;
vector<Platform> standin;
//setskillvalue
bool ultimateOn = false;
bool shieldOn = false;
bool isCharge = false;
bool isShield = false;
bool isEnchanted = false;
bool isBoosted = false;
bool alive = true;
bool gameon = false;
int score = 0;
int x = 1;
int currentlevel = 1;
Clock gametime;


vector<pair<string, int>> scorebar;
pair<string, int> tempText;
bool change = false;
int sortin(pair<string, int>a, pair<string, int>b){
	return a.second > b.second;
}

void cleardata();
int main()
{
	//setuptime
	srand(time(NULL));
	float time = 0.0f;
	float delayShoot;
	float delayEnemyShoot;
	float delayDeploy;
	float delayUltimate;
	float cooldown_Ultimate = 0;
	float count_Ultimate ;
	float count_chargeTime;
	float cooldown_Shield = 0;
	float count_shield;
	float count_shieldtime;
	float count_shieldStayTime;
	float cooldown_W=0;
	float count_W;
	float cooldown_Q = 0;
	float count_Q;
	float count_boost;
	float count_boss;
	float count_bossshot;
	
	float count_gametime;
	float Hshield=800;
	float count_back;
	Clock clock;
	Clock clock2;
	Clock clock3;
	Clock clock4;
	Clock clock5;
	Clock Cooldown;
	Clock Cooldown_Shield;
	Clock chargeTime;
	Clock back;
	Clock shieldTime;
	Clock shieldStayTime;
	Clock Wtime;
	Clock Qtime;
	Clock boosttime;
	Clock bosstime;
	Clock bossshottime;

	
	//loadfile
	Font font;
	font.loadFromFile("./Material/arial.ttf");
	if (!font.loadFromFile("./Material/arial.ttf"))
	{
		cout << "Failed to load file";
	}
	//sound
	SoundBuffer buttonHit;
	SoundBuffer shoot;
	SoundBuffer eshoot;
	SoundBuffer explosion;
	SoundBuffer ls;
	SoundBuffer sh;
	if (!buttonHit.loadFromFile("./sound/Menu.wav")) {
		return -1;
	}
	if (!shoot.loadFromFile("./sound/Gun.wav")) {
		return -1;
	}
	if (!eshoot.loadFromFile("./sound/enemyGun.wav")) {
		return -1;
	}
	if (!explosion.loadFromFile("./sound/explosion.wav")) {
		return -1;
	}
	if (!ls.loadFromFile("./sound/laser.wav")) {
		return -1;
	}
	if (!sh.loadFromFile("./sound/shield.wav")) {
		return -1;
	}
	Sound buttonSound;
	Sound shot;
	Sound eshot;
	Sound explode;
	Sound LS;
	Sound SH;
	
	buttonSound.setBuffer(buttonHit);
	shot.setBuffer(shoot);
	shot.setVolume(8.5);
	eshot.setBuffer(eshoot);
	explode.setBuffer(explosion);
	explode.setVolume(8.5);
	LS.setBuffer(ls);
	SH.setBuffer(sh);
	//music
	Music bgm;
	Music ingame;
	if (!bgm.openFromFile("./sound/MenuScreen.ogg")) {
		return -1;
	}
	if (!ingame.openFromFile("./sound/ingame.ogg")) {
		return -1;
	}
	bgm.setLoop(true);
	bgm.setPitch(1.2f);
	bgm.setVolume(40.0f);
	bgm.play();
	ingame.setLoop(true);
	ingame.setPitch(1.2f);
	ingame.setVolume(40.0f);
	//ingame.play();*/

	textbox TB(15, true);
	TB.setFont(font);
	TB.setPosition(Vector2f(-30, -30));
	Event ev;
	Event event_enter;

	// 1024 768
	RenderWindow window(VideoMode(1024, 950), "rtx3080", Style::Titlebar);
	View view(Vector2f(0.0f, 0.0f), Vector2f(VIEW_IN, VIEW_IN));
	Texture spaceShip,blueBullet,enemy,laser,redBullet,bodyLaser,enemyDestroyed,interfaceShip,shipDestroyed;
	spaceShip.loadFromFile("./Material/ship2.png");
	blueBullet.loadFromFile("./Material/fakeBlueBullet.png");
	enemy.loadFromFile("./Material/enemy.png");
	laser.loadFromFile("./Material/headUltimateLaser.png");
	bodyLaser.loadFromFile("./Material/ultimateLaser.png");
	redBullet.loadFromFile("./Material/fakeRedBullet.png");
	enemyDestroyed.loadFromFile("./Material/enemyShipDestroyed.png");
	interfaceShip.loadFromFile("./Material/interfaces.png");
	shipDestroyed.loadFromFile("./Material/shipDestroyed.png");
	Texture healthbar, manaShip;
	healthbar.loadFromFile("./Material/Healthbar.png");
	manaShip.loadFromFile("./Material/mana.png");
	Texture skill, skillused,charg;
	skill.loadFromFile("./Material/LuxMaliceCannon.png");
	skillused.loadFromFile("./Material/LuxMaliceCannonUsed.png");
	charg.loadFromFile("./Material/chargingLaser.png");
	Texture shieldCome,shieldOperate, skillShield, skillShield_Used,shieldGone,shieldBar;
	shieldCome.loadFromFile("./Material/shield.png");
	skillShield.loadFromFile("./Material/JarvanIVGoldenAegis.png");
	skillShield_Used.loadFromFile("./Material/JarvanIVGoldenAegisUsed.png");
	shieldGone.loadFromFile("./Material/shieldGone.png");
	shieldOperate.loadFromFile("./Material/shieldOperate.png");
	shieldBar.loadFromFile("./Material/shieldBar.png");
	Texture W, Wused, enchanted;
	W.loadFromFile("./Material/LucianR.png"); 
	Wused.loadFromFile("./Material/LucianRUsed.png");
	enchanted.loadFromFile("./Material/enchantedBullet.png");
	Texture Q, QUsed, Boost;
	Q.loadFromFile("./Material/ZileanW.png");
	QUsed.loadFromFile("./Material/ZileanWUsed.png");
	Boost.loadFromFile("./Material/Boost.png");
	Texture levels;
	levels.loadFromFile("./Material/levelbar.png");
	RectangleShape black;
	Texture boss,bossdead,bossbullet;
	boss.loadFromFile("./Material/Boss.png");
	bossdead.loadFromFile("./Material/Bossdead.png");
	bossbullet.loadFromFile("./Material/fakebossBullet.png");
	Texture menu, start, startpress, highscore, highscorepress,guide,guidepress,exit,exitpress;
	menu.loadFromFile("./Material/menu.png");
	start.loadFromFile("./Material/start.png");
	startpress.loadFromFile("./Material/startpressed.png");
	highscore.loadFromFile("./Material/highscore.png");
	highscorepress.loadFromFile("./Material/highscorepressed.png");
	guide.loadFromFile("./Material/guide.png");
	guidepress.loadFromFile("./Material/guidepressed.png");
	exit.loadFromFile("./Material/exit.png");
	exitpress.loadFromFile("./Material/exitpressed.png");
	Texture t1, t2, t3;
	t1.loadFromFile("./Material/t1.png");
	t2.loadFromFile("./Material/t2.png");
	t3.loadFromFile("./Material/t3.png");
	Texture Highscore_In, Guide_In, Background_In,enterName;
	Highscore_In.loadFromFile("./Material/showhighscore.png");
	Guide_In.loadFromFile("./Material/howtoplay.png");
	Background_In.loadFromFile("./Material/bg.jpg");
	enterName.loadFromFile("./Material/entername.png");
	Ship ship(&spaceShip, Vector2u(3, 3), 0.1f, 310.0f);
	//menudelay
	Clock calltime;
	Clock calltemp;
	Clock callmove;
	float count_calltime;
	float count_calltemp;
	float count_callmove;
	int stage = 1;
	Vector2i callstage (1,1);
	//windowgame
	window.setKeyRepeatEnabled(true);
	window.setFramerateLimit(30);
	
	while (window.isOpen())
	{	
		
			/*while (window.pollEvent(ev))
			{
				switch (ev.type)
				{
				case Event::Closed: window.close(); break;
				case Event::Resized: ReSizeView(window, view);
				}
			}*/
			
		
		if (stage == 1)
		{
				
				Platform bg(&menu, Vector2f(1080,720), Vector2f (0,40));
				Platform st(&start, Vector2f(100,50), Vector2f(-80, 100));
				Platform hs(&highscore, Vector2f(150, 50), Vector2f(80, 100));
				Platform gu(&guide, Vector2f(100, 50), Vector2f(-80, 160));
				Platform ex(&exit, Vector2f(100, 50), Vector2f(80, 160));
				
				count_calltime = calltime.getElapsedTime().asMilliseconds();
				count_calltemp = calltemp.getElapsedTime().asMilliseconds();
				count_callmove= callmove.getElapsedTime().asMilliseconds();

				ingame.pause();
				if (!bgm.Playing) {
					bgm.play();
				}
				//bgm.play();
				if (count_calltemp > 5000)
				{
					if (x == 1)
					{
						Platform stand(&t1, Vector2f(50, 70), Vector2f(-280,randomRange(-300,300)));
						standin.push_back(stand);
						//cout << "Push1" << endl;
					}
					else if (x == 2)
					{
						Platform stand(&t2, Vector2f(35, 50), Vector2f(-280, randomRange(-300, 300)));
						standin.push_back(stand);
						//cout << "Push2" << endl;
					}
					else
					{
						Platform stand(&t3, Vector2f(20, 30), Vector2f(-280, randomRange(-300, 300)));
						standin.push_back(stand);
						//cout << "Push3" << endl;
					}
					x++;
					if (x > 3) x = 1;
					calltemp.restart();
				}
				if (Keyboard::isKeyPressed(Keyboard::Right)&& count_calltime>200)
				{
					callstage.x++;
					if (callstage.x > 2) callstage.x -= 2;
					calltime.restart();
					buttonSound.play();
					/*stage = 2;
					window.clear();
					gametime.restart();
					cleardata();
					continue;*/
				}
				if (Keyboard::isKeyPressed(Keyboard::Left)&&count_calltime > 200)
				{
					callstage.x--;
					if (callstage.x < 1) callstage.x += 2;
					calltime.restart();
					buttonSound.play();
				}
				if (Keyboard::isKeyPressed(Keyboard::Up) && count_calltime > 200)
				{
					callstage.y--;
					if (callstage.y < 1) callstage.y += 2;
					calltime.restart();
					buttonSound.play();
					/*stage = 2;
					window.clear();
					gametime.restart();
					cleardata();
					continue;*/
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) && count_calltime > 200)
				{
					callstage.y++;
					if (callstage.y > 2) callstage.y -= 2;
					calltime.restart();
					buttonSound.play();
				}
				if (callstage.x+(2*callstage.y)== 3)
				{
					st.body.setTexture(&startpress);
				}
				else st.body.setTexture(&start);
				if (callstage.x + (2 * callstage.y) == 4)
				{
					hs.body.setTexture(&highscorepress);
				}
				else hs.body.setTexture(&highscore);
				if (callstage.x + (2 * callstage.y) == 5)
				{
					gu.body.setTexture(&guidepress);
				}
				else gu.body.setTexture(&guide);
				if (callstage.x + (2 * callstage.y) == 6)
				{
					ex.body.setTexture(&exitpress);
				}
				else ex.body.setTexture(&exit);
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					//bgm.stop();
					scorebar.clear();
					buttonSound.play();
					string Text;
					FILE* fp1;
					char c;

					fp1 = fopen("./text/score.txt", "r");
					while (1)
					{

						c = fgetc(fp1);
						if (c == EOF) {
							tempText.second /= 10;
							scorebar.push_back(tempText);
							tempText.first.clear();
							change = false;
							tempText.second = 0;
							break;
						}
						else {
							if (c == ' ') change = true;
							else if (c == '\n') {
								tempText.second /= 10;
								scorebar.push_back(tempText);
								tempText.first.clear();
								change = false;
								tempText.second = 0;
							}
							if (change) {
								if (c != ' ') {
									tempText.second += c - 48;
									tempText.second *= 10;
								}
							}
							else {
								tempText.first.push_back(c);
							}
						}
						//cout << '!'<<c;
					}
					fclose(fp1);
					stage = callstage.x + (2 * callstage.y);
					if (stage == 3) gameon = true;
					window.clear();
					gametime.restart();
					if (stage == 3) {
						cleardata();
					}
					continue;
		
				}
				//if(count_callmove>1000)
				for (Platform& Platforms : standin)
				{
					Platforms.body.setRotation(Platforms.rotation);
					Platforms.rotation++;
					Vector2f plattemp = Platforms.body.getPosition();
					Platforms.body.move(Vector2f(1.0f, 0));
				//	cout << plattemp.x  << ":" << plattemp.y << endl;
				//	cout << "Move" << endl;
					callmove.restart();

				}
				/*if (!standin.empty())
				{
					Platform stan = (*standin.begin());
					if (stan.body.getPosition().x > 260)
					{
						standin.erase(standin.begin());
					}
				}*/
			
				
				window.clear();
				window.setView(view);
				
				bg.Draw(window);
				for (Platform& Platforms : standin) Platforms.Draw(window);
				st.Draw(window);
				hs.Draw(window);
				gu.Draw(window);
				ex.Draw(window);
				
				window.display();
		}
		//settime;
		//if (!alive) cleardata();
		else if (stage == 7)
		{

		time = clock.restart().asSeconds();
		delayShoot = clock2.getElapsedTime().asMilliseconds();
		delayDeploy = clock3.getElapsedTime().asMilliseconds();
		delayUltimate = clock4.getElapsedTime().asMilliseconds();
		delayEnemyShoot = clock5.getElapsedTime().asMilliseconds();
		count_bossshot = bossshottime.getElapsedTime().asMilliseconds();
		count_Ultimate = Cooldown.getElapsedTime().asMilliseconds();
		count_chargeTime = chargeTime.getElapsedTime().asMilliseconds();
		count_gametime = gametime.getElapsedTime().asSeconds();
		count_shield = Cooldown_Shield.getElapsedTime().asMilliseconds();
		count_shieldtime = shieldTime.getElapsedTime().asMilliseconds();
		count_shieldStayTime = shieldStayTime.getElapsedTime().asMilliseconds();
		count_W = Wtime.getElapsedTime().asMilliseconds();
		count_Q = Qtime.getElapsedTime().asMilliseconds();
		count_boost = boosttime.getElapsedTime().asMilliseconds();
		count_boss = bosstime.getElapsedTime().asSeconds();
		count_back = back.getElapsedTime().asMilliseconds();
		//update status ship
		if (!isEnchanted)
		{
			ship.info_ally.atkSpd = 310 - (10 * ship.info_ally.level);
			ship.info_ally.dmg = 100 + (5 * ship.info_ally.level);
		}
		if (!isBoosted)
			ship.speed = 295 + (5 * ship.info_ally.level);
		ship.info_ally.maxHealth = 1000 + (200 * ship.info_ally.level);
		ship.info_ally.maxmana = 600 + (20 * ship.info_ally.level);
		if (currentlevel < ship.info_ally.level&&alive)
		{
			currentlevel = ship.info_ally.level;
			ship.info_ally.health = ship.info_ally.maxHealth;
			ship.info_ally.mana = ship.info_ally.maxmana;
		}
		//putobject
		Platform background_Game(&Background_In, Vector2f(1080, 720), Vector2f(0, 40));
		black.setFillColor(Color::Black);
		black.setSize(Vector2f(1500.0f, 150.0f));
		black.setPosition(Vector2f(-300.0f, 145.0f));
		//black.setOrigin(black.getPosition());
		platforms.push_back(Platform(&interfaceShip, Vector2f(500.0f, 100.0f), Vector2f(0.0f, 200.0f)));
		if (!interface_ShipHealth.empty()) interface_ShipHealth.pop_back();
		if (ship.GetHealth() > 0 && ship.GetHealth() <= ship.GetMaxHealth())
		{
			float current = 287 * ship.GetHealth() / ship.GetMaxHealth();
			Platform hb(&healthbar, Vector2f(current, 8.0f), Vector2f(232.0f - (287 - current), 226.0f));
			hb.body.setOrigin(hb.body.getSize());
			interface_ShipHealth.push_back(hb);
		}
		if (!Levels.empty()) Levels.pop_back();
		if (ship.info_ally.expBar > 0 && ship.info_ally.expBar <= ship.info_ally.levelGate)
		{
			float current;
			if (ship.info_ally.expBar <= ship.info_ally.levelGate)
				current = 80 * ship.info_ally.expBar / ship.info_ally.levelGate;
			else
				current = 80 * (ship.info_ally.expBar - ship.info_ally.levelGate) / ship.info_ally.levelGate;
			Platform lv(&levels, Vector2f(28.0f, current), Vector2f(-65.0f, 230.0f));
			lv.body.setOrigin(lv.body.getSize());
			Levels.push_back(lv);
		}
		if (!interface_ShipshieldBar.empty()) interface_ShipshieldBar.pop_back();
		if (!shields.empty())
		{
			if ((*shields.begin()).health > 0)
			{
				float current = 287 * (*shields.begin()).health / (800+(100*ship.info_ally.level));
				Platform sb(&shieldBar, Vector2f(current, 8.0f), Vector2f(232.0f - (287 - current), 226.0f));
				sb.body.setOrigin(sb.body.getSize());
				interface_ShipshieldBar.push_back(sb);
			}
		}
		if (!interface_ShipMana.empty()) interface_ShipMana.pop_back();
		if (ship.GetMana() > 0 && ship.GetMana() <= ship.GetMaxMana())
		{
			float current = 287 * ship.GetMana() / ship.GetMaxMana();
			Platform mn(&manaShip, Vector2f(current, 8.0f), Vector2f(232.0f - (287 - current), 240.0f));
			mn.body.setOrigin(mn.body.getSize());
			interface_ShipMana.push_back(mn);
		}
		if (!interface_ShipSkill.empty()) interface_ShipSkill.pop_back();
		if (cooldown_Ultimate == 0 && ship.GetMana() >= 500 && ship.info_ally.level >= 6)
		{
			Platform sk(&skill, Vector2f(34.0f, 32.0f), Vector2f(139.0f, 185.0f));
			interface_ShipSkill.push_back(sk);
		}
		else
		{
			Platform sk(&skillused, Vector2f(34.0f, 32.0f), Vector2f(139.0f, 185.0f));
			interface_ShipSkill.push_back(sk);
		}
		if (!interface_Shipshield.empty()) interface_Shipshield.pop_back();
		if (cooldown_Shield == 0 && ship.GetMana() >= 300 && ship.info_ally.level >= 4)
		{
			Platform sk(&skillShield, Vector2f(34.0f, 32.0f), Vector2f(93.0f, 183.0f));
			interface_Shipshield.push_back(sk);
		}
		else
		{
			Platform sk(&skillShield_Used, Vector2f(34.0f, 32.0f), Vector2f(93.0f, 183.0f));
			interface_Shipshield.push_back(sk);
		}
		if (!interface_Shipenchant.empty()) interface_Shipenchant.pop_back();
		if (cooldown_W == 0 && ship.GetMana() >= 150 && ship.info_ally.level >= 2)
		{
			Platform sk(&W, Vector2f(34.0f, 32.0f), Vector2f(45.0f, 183.0f));
			interface_Shipenchant.push_back(sk);
		}
		else
		{
			Platform sk(&Wused, Vector2f(34.0f, 32.0f), Vector2f(45.0f, 183.0f));
			interface_Shipenchant.push_back(sk);
		}
		if (!interface_Boosts.empty()) interface_Boosts.pop_back();
		if (cooldown_Q == 0 && ship.GetMana() >= 100)
		{
			Platform sk(&Q, Vector2f(34.0f, 32.0f), Vector2f(0.0f, 183.0f));
			interface_Boosts.push_back(sk);
		}
		else
		{
			Platform sk(&QUsed, Vector2f(34.0f, 32.0f), Vector2f(0.0f, 183.0f));
			interface_Boosts.push_back(sk);
		}
		//shoot bullet
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (delayShoot > ship.info_ally.atkSpd)
			{
				if (!isEnchanted)
				{
					Vector2f tempPosition = ship.GetPosition();
					Bullet temp(&blueBullet, Vector2u(2, 1), 0.1f, Vector2f(tempPosition.x, tempPosition.y - 50.0f));
					temp.dmg = ship.info_ally.dmg;
					bullets.push_back(temp);
					shot.play();
					clock2.restart();
				}
				else
				{
					Vector2f tempPosition = ship.GetPosition();

					Bullet temp(&enchanted, Vector2u(2, 1), 0.1f, Vector2f(tempPosition.x, tempPosition.y - 50.0f));

					temp.dmg = ship.info_ally.dmg;
					bullets.push_back(temp);
					shot.play();
					clock2.restart();
				}
			}
		}
		//skill 1 
		if (Keyboard::isKeyPressed(Keyboard::Q))
		{
			if (ship.GetMana() >= 150 && cooldown_Q == 0 && !isBoosted)
			{
				isBoosted = true;
				ship.info_ally.mana -= 150;
				Vector2f tempPosition = ship.GetPosition();
				Ship temp2(&Boost, Vector2u(3, 3), 0.1f, 300.0f);
				temp2.speed = (300 + (10 * ship.info_ally.level)) * 2;
				temp2.body.setPosition(tempPosition);
				boosts.push_back(temp2);
				cooldown_Q = 5;
				Qtime.restart();
				boosttime.restart();
			}
		}
		if (!isBoosted)
		{
			boosttime.restart();
		}
		if (isBoosted)
		{
			ship.speed = (300 + (10 * ship.info_ally.level)) * 1.6;
			//cout << count_boost << endl;
		}
		if (count_boost > 3000 && isBoosted)
		{
			isBoosted = false;
			boosts.pop_back();
			boosttime.restart();
			//	cout << "pop" << endl;
		}

		//skill 2
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			if (ship.GetMana() >= 200 && cooldown_W == 0 && !isEnchanted && ship.info_ally.level >= 2)
			{
				ship.info_ally.mana -= 200;
				isEnchanted = true;
				cooldown_W = 7;
				Wtime.restart();
			}
		}
		if (isEnchanted)
		{
			ship.info_ally.dmg = (100 + (20 * ship.info_ally.level)) * 1.4;
		}
		if (cooldown_W < 2 && isEnchanted)
		{
			isEnchanted = false;

		}

		//skill 3 
		if (Keyboard::isKeyPressed(Keyboard::E))
		{
			if (ship.GetMana() >= 300 && cooldown_Shield == 0 && !isShield && ship.info_ally.level >= 4)
			{
				Vector2f tempPosition = ship.GetPosition();
				Eliminate temp(&shieldCome, Vector2u(10, 1), 0.1f, Vector2f(tempPosition.x - 20.0f, tempPosition.y - 25.0f));
				temp.body.setSize(Vector2f(80.0f, 100.0f));
				shieldGone_animated.push_back(temp);
				isShield = true;
				shieldOn = true;
				ship.info_ally.mana -= 300;
				if (ship.info_ally.mana < 0) ship.info_ally.mana = 0;
				shieldTime.restart();
				Cooldown_Shield.restart();
				shieldStayTime.restart();
				SH.play();
				//	cout << "push"<<endl;
			}
		}
		if (count_shieldtime > 1000)
		{
			if (isShield)
			{
				isShield = false;
				shieldGone_animated.pop_back();

				if (shieldOn)
				{
					Vector2f tempPosition = ship.GetPosition();
					Skill temp2(&shieldOperate, Vector2u(3, 1), 0.1f, Vector2f(tempPosition.x, tempPosition.y), 300.0f, Vector2f(80.0f, 100.0f));
					//temp2.body.setOrigin(ship.body.getOrigin());
					temp2.health = 800 + (100 * ship.info_ally.level);
					shields.push_back(temp2);
					cooldown_Shield = 10;
					Cooldown_Shield.restart();
					shieldStayTime.restart();
					//		cout << "pushshield" << endl;
				}

			}

			shieldTime.restart();

		}

		if (!shields.empty()) Hshield = (*shields.begin()).Gethealth();
		//cout <<H<<' ' << isShield << ' ' << shieldOn << ' ' <<count_shieldStayTime << endl;
		if (shieldOn && !shields.empty())
		{
			if ((Hshield <= 0 && !isShield) || (shieldOn && count_shieldStayTime > 5000))
			{
				shields.pop_back();
				Vector2f tempPosition = ship.GetPosition();
				Eliminate temp(&shieldGone, Vector2u(10, 1), 0.1f, Vector2f(tempPosition.x - 20.0f, tempPosition.y - 25.0f));
				temp.body.setOrigin(ship.body.getOrigin());
				temp.body.setSize(Vector2f(80.0f, 100.0f));
				shieldGone_animated.push_back(temp);
				Cooldown_Shield.restart();
				shieldTime.restart();
				shieldOn = false;
				isShield = true;
				//	cout << "shieldgone" << endl;
			}

		}
		//ultimateskill
		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			if (ship.GetMana() >= 500 && cooldown_Ultimate == 0 && !isCharge && ship.info_ally.level >= 6)
			{
				Vector2f tempPosition = ship.GetPosition();
				isCharge = true;
				Eliminate temp3(&charg, Vector2u(5, 1), 0.8f, Vector2f(tempPosition.x - 27.0f, tempPosition.y - 50.0f));
				temp3.body.setSize(Vector2f(80.0f, 40.0f));
				charging.push_back(temp3);
				ultimateOn = true;
				ship.info_ally.mana -= 500;
				if (ship.info_ally.mana < 0) ship.info_ally.mana = 0;
				chargeTime.restart();
				LS.play();
				clock4.restart();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			alive = false;
			gameon = false;
			gametime.restart();
			cleardata();
			stage = 1;
			continue;
		}
		if (count_chargeTime > 4000)
		{
			if (isCharge)
			{
				isCharge = false;
				Vector2f tempPosition = ship.GetPosition();
				Skill temp(&laser, Vector2u(2, 1), 0.1f, Vector2f(tempPosition.x - 1.0f, tempPosition.y - 50.0f), ship.speed, Vector2f(80.0f, 40.0f));
				Skill temp2(&bodyLaser, Vector2u(1, 1), 0.1f, Vector2f(tempPosition.x + 1.0f, tempPosition.y - 340.0f), ship.speed, Vector2f(14.4f, 550.0f));
				temp.dmg = ship.info_ally.dmg;
				temp2.dmg = ship.info_ally.dmg;
				skills.push_back(temp);
				skills.push_back(temp2);
				cooldown_Ultimate = 40;
				Cooldown.restart();
				//clock4.restart();
			}
			chargeTime.restart();
		}

		//cout << isCharge<<" : "<<count_chargeTime <<" : " <<cooldown_Ultimate<< endl;
		//checkultimate
		if (ultimateOn)
		{
			if (delayUltimate > 5500 && !isCharge)
			{
				skills.erase(skills.begin());
				skills.erase(skills.begin());
				charging.erase(charging.begin());
				clock4.restart();
				ultimateOn = false;
			}
		}
		//cout << count_gametime << endl;
		//spawn enemy
		if (delayDeploy > 2000 - (10 * count_gametime) && gameon && count_gametime > 3 && enemys.size()<10)
		{
			Enemy temp(&enemy, Vector2u(3, 3), 0.1f, Vector2f(randomRange(-250, 250), -300.0f));
			temp.info_enemy.maxHealth = 400 + (1 * count_gametime);
			temp.info_enemy.health = temp.info_enemy.maxHealth;
			temp.dmg = 800 + (1 * count_gametime);
			float Health = temp.info_enemy.health;
			float Maxhealth = temp.info_enemy.maxHealth;
			enemys.push_back(temp);
			clock3.restart();
		}
		vector<Enemy>::iterator EnemyItr = enemys.begin();
		while (EnemyItr != enemys.end()){
			Enemy e = *EnemyItr;
			if (e.GetPosition().y > 300) {
				EnemyItr = enemys.erase(EnemyItr);
			}
			else EnemyItr++;
		}
		if (count_boss > 40 - (0.2 * count_gametime)&& bosss.size()<3)
		{
			Boss temp(&boss, Vector2u(4, 1), 0.1f, Vector2f(0, -400.0f)); //-220
			temp.info_boss.maxHealth = 4000 + (10 * count_gametime);
			temp.info_boss.health = temp.info_boss.maxHealth;
			temp.dmg = 800 + (10 * count_gametime);
			bosss.push_back(temp);
			bosstime.restart();
		}
		//randombullet
		for (Enemy& Enemys : enemys)
		{
			if (delayEnemyShoot > 3000 - count_gametime)
			{
				if (rand() % 100 > 40)
				{
					Vector2f tempPosition = Enemys.GetPosition();
					EnemyBullet temp(&redBullet, Vector2u(2, 1), 0.1f, Vector2f(tempPosition.x, tempPosition.y + 50.0f));
					temp.dmg += count_gametime;
					enemybullets.push_back(temp);
					eshot.play();
					clock5.restart();
				}
			}
		}
		for (Boss& Enemys : bosss)
		{
			if (count_bossshot > 500)
			{
				if (rand() % 100 > 10)
				{
					Vector2f tempPosition = Enemys.GetPosition();
					EnemyBullet temp(&bossbullet, Vector2u(2, 1), 0.1f, Vector2f(tempPosition.x, tempPosition.y + 50.0f));
					temp.dmg = 70 + count_gametime;
					temp.body.setSize(Vector2f(15, 15));
					enemybullets.push_back(temp);
					eshot.play();
					bossshottime.restart();
				}
			}
		}
		//cout << delayUltimate << " : " << ultimateOn<<endl;
		//check collition
		if (alive)
		{
			vector<Enemy>::iterator enemyItr = enemys.begin();
			while (enemyItr != enemys.end())
			{
				bool checkCollitionByLaser = false;
				bool checkCollitionByBullet = false;
				vector<Bullet>::iterator bulletItr = bullets.begin();
				Enemy tempEnemy = *enemyItr;
				float bulletDmg, laserDmg;
				while (bulletItr != bullets.end())
				{
					Bullet tempBullet = *bulletItr;


					Collider bulletHit = tempBullet.GetCollider();
					if (tempEnemy.GetCollider().CheckCollision(bulletHit))
					{
						if (ship.info_ally.mana > ship.GetMaxMana()) ship.info_ally.mana = ship.GetMaxMana();
						bulletDmg = (*bulletItr).dmg;
						bulletItr = bullets.erase(bulletItr);
						checkCollitionByBullet = true;
					}

					else
						bulletItr++;
				}
				if (ultimateOn && !isCharge)
				{
					Skill tempskill = *skills.begin();
					Skill tempskill2 = *(skills.begin() + 1);
					Collider laserHit = tempskill.GetCollider();
					Collider laserHit2 = tempskill2.GetCollider();
					if (tempEnemy.GetCollider().CheckCollision(laserHit) || tempEnemy.GetCollider().CheckCollision(laserHit2))
					{

						if (ship.info_ally.mana > ship.GetMaxMana()) ship.info_ally.mana = ship.GetMaxMana();
						laserDmg = (*skills.begin()).dmg;
						checkCollitionByLaser = true;
					}
				}
				if (checkCollitionByBullet || checkCollitionByLaser)
				{
					if (checkCollitionByBullet) (*enemyItr).info_enemy.health -= bulletDmg;
					if (checkCollitionByLaser) (*enemyItr).info_enemy.health -= laserDmg;
					if ((*enemyItr).info_enemy.health <= 0)
					{
						Vector2f tempPosition = tempEnemy.GetPosition();
						Vector2f tempSize = tempEnemy.GetSize();
						Eliminate dead(&enemyDestroyed, Vector2u(5, 1), 0.15f, Vector2f(tempPosition.x, tempPosition.y));
						//cout << tempEnemy.GetSize().x << " Six:Siy " << tempEnemy.GetSize().y<< tempEnemy.GetPosition().x << " Pox:Poy " << tempEnemy.GetPosition().y<<endl;
						players.push_back(dead);
						explode.play();
						score += 100;
						ship.info_ally.mana += 15;
						ship.info_ally.expBar += 200;
						if (ship.info_ally.mana > ship.info_ally.maxmana) ship.info_ally.mana = ship.info_ally.maxmana;
						enemyItr = enemys.erase(enemyItr);
					}
				}
				else enemyItr++;
				bulletItr = bullets.begin();
			}
			vector<Boss>::iterator bossItr = bosss.begin();
			while (bossItr != bosss.end())
			{
				bool checkCollitionByLaser = false;
				bool checkCollitionByBullet = false;
				vector<Bullet>::iterator bulletItr = bullets.begin();
				Boss tempBoss = *bossItr;
				float bulletDmg, laserDmg;
				while (bulletItr != bullets.end())
				{
					Bullet tempBullet = *bulletItr;
					Collider bulletHit = tempBullet.GetCollider();
					if (tempBoss.GetCollider().CheckCollision(bulletHit))
					{
						if (ship.info_ally.mana > ship.GetMaxMana()) ship.info_ally.mana = ship.GetMaxMana();
						bulletDmg = (*bulletItr).dmg;
						bulletItr = bullets.erase(bulletItr);
						checkCollitionByBullet = true;
					}

					else
						bulletItr++;
				}
				if (ultimateOn && !isCharge)
				{
					Skill tempskill = *skills.begin();
					Skill tempskill2 = *(skills.begin() + 1);
					Collider laserHit = tempskill.GetCollider();
					Collider laserHit2 = tempskill2.GetCollider();
					if (tempBoss.GetCollider().CheckCollision(laserHit) || tempBoss.GetCollider().CheckCollision(laserHit2))
					{

						if (ship.info_ally.mana > ship.GetMaxMana()) ship.info_ally.mana = ship.GetMaxMana();
						laserDmg = (*skills.begin()).dmg;
						checkCollitionByLaser = true;
					}
				}
				if (checkCollitionByBullet || checkCollitionByLaser)
				{
					if (checkCollitionByBullet) (*bossItr).info_boss.health -= bulletDmg;
					if (checkCollitionByLaser)
					{
						(*bossItr).info_boss.health -= laserDmg;
					}
					if ((*bossItr).info_boss.health <= 0)
					{
						Vector2f tempPosition = (*bossItr).GetPosition();
						Eliminate dead(&bossdead, Vector2u(4, 1), 0.1f, Vector2f(((*bossItr).GetPosition().x) - 53, -280));
						dead.body.setSize(Vector2f(187.0f, 262.0f));
						//cout << tempPosition.x << " Pox:Poy " << tempPosition.y << endl;
						players.push_back(dead);
						explode.play();
						score += 300;
						ship.info_ally.mana += 25;
						if (ship.info_ally.mana > ship.info_ally.maxmana) ship.info_ally.mana = ship.info_ally.maxmana;
						ship.info_ally.expBar += 350;
						bossItr = bosss.erase(bossItr);
					}
				}
				else bossItr++;
				bulletItr = bullets.begin();
			}
			vector<EnemyBullet>::iterator enemyBulletItr = enemybullets.begin();

			while (enemyBulletItr != enemybullets.end())
			{
				float bulletDmg;
				bool checkCollitionByBullet = false;
				EnemyBullet tempBullet = *enemyBulletItr;


				Collider bulletHit = tempBullet.GetCollider();
				if (!shields.empty() && shieldOn)
				{
					if ((*shields.begin()).GetCollider().CheckCollision(bulletHit))
					{
						bulletDmg = (*enemyBulletItr).dmg;
						enemyBulletItr = enemybullets.erase(enemyBulletItr);
						checkCollitionByBullet = true;
					}
					else enemyBulletItr++;
				}
				else
				{
					if (ship.GetCollider().CheckCollision(bulletHit))
					{
						bulletDmg = (*enemyBulletItr).dmg;
						enemyBulletItr = enemybullets.erase(enemyBulletItr);
						checkCollitionByBullet = true;
					}

					else
						enemyBulletItr++;
				}
				if (checkCollitionByBullet)
				{
					if (shieldOn && !shields.empty())
					{
						if (bulletDmg > (*shields.begin()).health)
						{
							ship.info_ally.health -= bulletDmg - (*shields.begin()).health;
							(*shields.begin()).health -= bulletDmg;
						}
						else
							(*shields.begin()).health -= bulletDmg;
					}
					else
						ship.info_ally.health -= bulletDmg;
					if (ship.info_ally.health <= 0)
					{
						explode.play();
						Vector2f tempPosition = ship.GetPosition();
						Vector2f tempSize = ship.GetSize();
						Eliminate dead(&shipDestroyed, Vector2u(5, 1), 0.15f, Vector2f(tempPosition.x, tempPosition.y));
						//cout << tempEnemy.GetSize().x << " Six:Siy " << tempEnemy.GetSize().y<< tempEnemy.GetPosition().x << " Pox:Poy " << tempEnemy.GetPosition().y<<endl;
						players.push_back(dead);
						
					}
				}
				if (ship.info_ally.health <= 0 && alive)
				{
					Vector2f tempPosition = ship.GetPosition();
					Vector2f tempSize = ship.GetSize();
					Eliminate dead(&shipDestroyed, Vector2u(5, 1), 0.25f, Vector2f(tempPosition.x, tempPosition.y));
					//cout << tempEnemy.GetSize().x << " Six:Siy " << tempEnemy.GetSize().y<< tempEnemy.GetPosition().x << " Pox:Poy " << tempEnemy.GetPosition().y<<endl;
					players.push_back(dead);
					explode.play();
					alive = false;
					gameon = false;
					gametime.restart();

				}
			}
			vector<Enemy>::iterator enemysItr = enemys.begin();
			while (enemysItr != enemys.end())
			{
				float bodyDmg;
				bool checkCollitionByBody = false;
				Enemy tempBullet = *enemysItr;


				Collider bulletHit = tempBullet.GetCollider();
				if (!shields.empty())
				{
					if ((*shields.begin()).GetCollider().CheckCollision(bulletHit))
					{
						bodyDmg = (*enemysItr).dmg;
						Vector2f tempPosition = (*enemysItr).GetPosition();
						Vector2f tempSize = (*shields.begin()).GetSize();
						Eliminate dead(&enemyDestroyed, Vector2u(5, 1), 0.15f, Vector2f(tempPosition.x, tempPosition.y));
						//cout << tempEnemy.GetSize().x << " Six:Siy " << tempEnemy.GetSize().y<< tempEnemy.GetPosition().x << " Pox:Poy " << tempEnemy.GetPosition().y<<endl;
						players.push_back(dead);
						enemysItr = enemys.erase(enemysItr);
						checkCollitionByBody = true;
					}
					else enemysItr++;
				}
				else
				{
					if (ship.GetCollider().CheckCollision(bulletHit))
					{
						bodyDmg = (*enemysItr).dmg;
						Vector2f tempPosition = (*enemysItr).GetPosition();
						Vector2f tempSize = ship.GetSize();
						Eliminate dead(&enemyDestroyed, Vector2u(5, 1), 0.15f, Vector2f(tempPosition.x, tempPosition.y));
						explode.play();
						score += 100;
						ship.info_ally.expBar += 250;
						//cout << tempEnemy.GetSize().x << " Six:Siy " << tempEnemy.GetSize().y<< tempEnemy.GetPosition().x << " Pox:Poy " << tempEnemy.GetPosition().y<<endl;
						players.push_back(dead);
						enemysItr = enemys.erase(enemysItr);
						checkCollitionByBody = true;
					}
					//set snemy dead by smash ship
					else
						enemysItr++;
				}
				if (checkCollitionByBody)
				{
					if (shieldOn && !shields.empty())
					{
						if (bodyDmg > (*shields.begin()).health)
						{
							ship.info_ally.health -= bodyDmg - (*shields.begin()).health;
							(*shields.begin()).health = 0;
						}
						else
							(*shields.begin()).health -= bodyDmg;
					}
					else ship.info_ally.health -= bodyDmg;
					
				}
			}
		}
		//text 
		Text health_text, mana_text, cd, skillCost, shield_cd, shieldCost, W_cd, Wcost, Q_cd, Qcost, L;
		Text ATK, ATK_SPD, MOVE;
		Text AleartW, AleartE, AleartR, AleartW2, AleartE2, AleartR2;
		health_text.setString(str((int)ship.GetHealth()) + "/" + str(ship.GetMaxHealth()));
		mana_text.setString(str(ship.GetMana()) + "/" + str(ship.GetMaxMana()));
		cd.setString(str(cooldown_Ultimate));
		skillCost.setString("500");
		shield_cd.setString(str(cooldown_Shield));
		shieldCost.setString("300");
		W_cd.setString(str(cooldown_W));
		Wcost.setString("150");
		Q_cd.setString(str(cooldown_Q));
		Qcost.setString("100");
		L.setString(str(ship.info_ally.level));
		ATK.setString(str(ship.info_ally.dmg));
		ATK_SPD.setString(str(200 + (200.0f - ship.info_ally.atkSpd)) + "%");
		MOVE.setString(str(ship.speed));
		AleartW.setString("Unlock");
		AleartE.setString("Unlock");
		AleartR.setString("Unlock");
		AleartW2.setString("LV 2");
		AleartE2.setString("LV 4");
		AleartR2.setString("LV 6");

		health_text.setFont(font);
		mana_text.setFont(font);
		cd.setFont(font);
		skillCost.setFont(font);
		shield_cd.setFont(font);
		shieldCost.setFont(font);
		W_cd.setFont(font);
		Wcost.setFont(font);
		Q_cd.setFont(font);
		Qcost.setFont(font);
		L.setFont(font);
		ATK.setFont(font);
		ATK_SPD.setFont(font);
		MOVE.setFont(font);
		AleartW.setFont(font);
		AleartE.setFont(font);
		AleartR.setFont(font);
		AleartW2.setFont(font);
		AleartE2.setFont(font);
		AleartR2.setFont(font);

		health_text.setCharacterSize(9);
		mana_text.setCharacterSize(9);
		cd.setCharacterSize(20);
		skillCost.setCharacterSize(10);
		shield_cd.setCharacterSize(20);
		shieldCost.setCharacterSize(10);
		W_cd.setCharacterSize(20);
		Wcost.setCharacterSize(10);
		Q_cd.setCharacterSize(20);
		Qcost.setCharacterSize(10);
		L.setCharacterSize(13);
		ATK.setCharacterSize(11);
		ATK_SPD.setCharacterSize(11);
		MOVE.setCharacterSize(11);
		AleartW.setCharacterSize(10);
		AleartE.setCharacterSize(10);
		AleartR.setCharacterSize(10);
		AleartW2.setCharacterSize(10);
		AleartE2.setCharacterSize(10);
		AleartR2.setCharacterSize(10);

		health_text.setPosition(60.0f, 217.0f);
		mana_text.setPosition(60.0f, 231.0f);
		cd.setPosition(129.0f, 170.0f);
		skillCost.setPosition(140.0f, 190.0f);
		shield_cd.setPosition(87.0f, 170.0f);
		shieldCost.setPosition(93.0f, 190.0f);
		W_cd.setPosition(40.0f, 170.0f);
		Wcost.setPosition(45.0f, 189.0f);
		Q_cd.setPosition(-5.0f, 170.0f);
		Qcost.setPosition(-1.0f, 190.0f);
		L.setPosition(-95.0f, 223.0f);
		ATK.setPosition(-230.0f, 190.0f);
		ATK_SPD.setPosition(-190.0f, 190.0f);
		MOVE.setPosition(-230.0f, 210.0f);
		AleartW.setPosition(30.0f, 170.0f);
		AleartE.setPosition(77.0f, 170.0f);
		AleartR.setPosition(122.0f, 170.0f);
		AleartW2.setPosition(35.0f, 180.0f);
		AleartE2.setPosition(82.0f, 180.0f);
		AleartR2.setPosition(129.0f, 180.0f);

		health_text.setFillColor(Color::White);
		mana_text.setFillColor(Color::White);
		cd.setFillColor(Color::White);
		shield_cd.setFillColor(Color::White);
		shieldCost.setFillColor(Color::White);
		W_cd.setFillColor(Color::White);
		Wcost.setFillColor(Color::White);
		Q_cd.setFillColor(Color::White);
		Qcost.setFillColor(Color::White);
		L.setFillColor(Color::White);
		ATK.setFillColor(Color::White);
		ATK_SPD.setFillColor(Color::White);
		MOVE.setFillColor(Color::White);
		AleartW.setFillColor(Color::White);
		AleartE.setFillColor(Color::White);
		AleartR.setFillColor(Color::White);
		AleartW2.setFillColor(Color::White);
		AleartE2.setFillColor(Color::White);
		AleartR2.setFillColor(Color::White);
		//score = 10000;
		Text SC;
		
		SC.setString(str(score));
		SC.setCharacterSize(30);
		SC.setFillColor(Color::White);
		SC.setPosition(Vector2f(200, -250));
		SC.setFont(font);
		if (score >= 10000) SC.move(-30, 0);
		else if (score >= 1000) SC.move(-15, 0);
		
		//update
		Collider tempShip = (*platforms.begin()).GetCollider();

		if (!skills.empty() && !isCharge)
		{
			Collider tempShip1 = ship.GetCollider();
			if (!(*skills.begin()).GetCollider().CheckCollision(tempShip1))
			{
				(*skills.begin()).Update(time);
			}
			else
			{
				(*skills.begin()).velocity.y -= ship.speed;
				(*skills.begin()).body.move(ship.velocity * time);
			}
			Collider tempShip2 = (*skills.begin()).GetCollider();
			if (!(*(skills.begin() + 1)).GetCollider().CheckCollision(tempShip2))
			{
				(*(skills.begin() + 1)).Update(time);
			}
			else
			{
				(*(skills.begin() + 1)).velocity.y -= ship.speed;
				(*(skills.begin() + 1)).body.move(ship.velocity * time);
			}
		}
		if (!shields.empty() && !isShield)
		{
			Collider tempshield = (*platforms.begin()).GetCollider();
			if (!(*shields.begin()).GetCollider().CheckCollision(tempshield))
			{
				(*shields.begin()).body.setPosition(ship.GetPosition());
				(*shields.begin()).Update(time);
			}
			else
			{
				(*shields.begin()).body.setPosition(ship.GetPosition());
				(*shields.begin()).Update(time);
				/*	Vector2f temp = ship.velocity;
					temp.y -= ship.speed;
					ship.velocity.y -= ship.speed;
					(*shields.begin()).body.move(temp * time);*/
			}
			//cout << '1' << endl;
		}
		else if (isShield)
		{
			Collider tempshield = (*platforms.begin()).GetCollider();
			if (!(*shieldGone_animated.begin()).GetCollider().CheckCollision(tempshield))
			{
				(*shieldGone_animated.begin()).body.setPosition(Vector2f(ship.GetPosition().x - 25.0f, ship.GetPosition().y - 29.0f));
				(*shieldGone_animated.begin()).Update(time);
			}
			else
			{
				/*Vector2f temp = ship.velocity;
				temp.y -= ship.speed;
				ship.velocity.y -= ship.speed;
				(*shieldGone_animated.begin()).body.move(temp * time);*/
				(*shieldGone_animated.begin()).body.setPosition(Vector2f(ship.GetPosition().x - 25.0f, ship.GetPosition().y - 29.0f));
				(*shieldGone_animated.begin()).Update(time);
			}
			//cout << '2' << endl;
		}
		if (!ship.GetCollider().CheckCollision(tempShip))
		{
			ship.Update(time);
		}
		else
		{
			ship.velocity.y -= ship.speed;
			ship.body.move(ship.velocity * time);
		}
		for (Bullet& Bullets : bullets) Bullets.Update(time);
		for (Enemy& Enemys : enemys)Enemys.Update(time);
		for (Boss& Bosss : bosss)Bosss.Update(time);
		for (EnemyBullet& Enemybullets : enemybullets)Enemybullets.Update(time);
		for (Eliminate& Videoplayers : charging)
		{
			Videoplayers.Update(time);
			(*charging.begin()).body.move(ship.velocity * time);
		}
		for (Eliminate& Videoplayers : players) Videoplayers.Update(time);
		for (Ship& Skills : boosts)
		{
			Skills.speed = ship.speed;
			Skills.Update(time);
			//	cout << "up" << endl;
		}

		vector<Eliminate>::iterator playerItr = players.begin();
		while (playerItr != players.end())
		{
			Eliminate temp = *playerItr;
			temp.Update(time);
			if (temp.getValue())
			{
				playerItr = players.erase(playerItr);
			}

			else playerItr++;
		}
		if (cooldown_Ultimate > 0)
		{
			if (count_Ultimate > 1000)
			{
				cooldown_Ultimate -= 1;
				Cooldown.restart();
			}
		}
		else cooldown_Ultimate = 0;
		if (cooldown_Shield > 0)
		{
			if (count_shield > 1000)
			{
				cooldown_Shield -= 1;
				Cooldown_Shield.restart();
			}
		}
		else cooldown_Shield = 0;
		if (cooldown_W > 0)
		{
			if (count_W > 1000)
			{
				cooldown_W -= 1;
				Wtime.restart();
			}
		}
		else cooldown_W = 0;
		if (cooldown_Q > 0)
		{
			if (count_Q > 1000)
			{
				cooldown_Q -= 1;
				Qtime.restart();
			}
		}
		else cooldown_Q = 0;
		//display
		window.clear();
		window.setView(view);
		background_Game.Draw(window);
		if (alive)
		{
			if (ship.GetPosition().x < -236) ship.body.setPosition(-236, ship.GetPosition().y);
			if (ship.GetPosition().x > 236) ship.body.setPosition(236, ship.GetPosition().y);
			if (ship.GetPosition().y < -236) ship.body.setPosition(ship.GetPosition().x, -236);
			ship.Draw(window);
		}
		if (isBoosted && alive)
			for (Ship& Skills : boosts)
			{
				Skills.body.setPosition(ship.GetPosition());
				Skills.Draw(window);
				//	cout << "draw" << endl;
			}
		
		for (Enemy& Enemys : enemys) Enemys.Draw(window);
		for (Boss& Bosss : bosss)Bosss.Draw(window);
		if(alive)
		{
		for (Bullet& Bullets : bullets) Bullets.Draw(window);
		if (!isCharge)
		{
			Vector2f tempPosition = ship.GetPosition();
			if (!skills.empty())
			{
				(*(skills.begin() + 1)).body.setPosition(Vector2f(tempPosition.x + 1.0f, tempPosition.y - 340.0f));
			}
			for (Skill& Skills : skills)
				Skills.Draw(window);
		}
		if (!isShield && alive)
		{
			for (Skill& Skills : shields)Skills.Draw(window);
		}
		
		for (Eliminate& Videoplayers : charging)
		{
			Videoplayers.Draw(window);
		}
		}
		for (Eliminate& Videoplayers : players)
		{
			Videoplayers.Draw(window);
		}

		for (EnemyBullet& Enemybullets : enemybullets)Enemybullets.Draw(window);
		for (Eliminate& Platforms : shieldGone_animated) Platforms.Draw(window);
		window.draw(black);
		for (Platform& Platforms : Levels) Platforms.Draw(window);
		(*platforms.begin()).Draw(window);
		for (Platform& Platforms : interface_ShipSkill) Platforms.Draw(window);
		for (Platform& Platforms : interface_Shipshield) Platforms.Draw(window);
		for (Platform& Platforms : interface_ShipHealth) Platforms.Draw(window);
		for (Platform& Platforms : interface_ShipMana) Platforms.Draw(window);
		for (Platform& Platforms : interface_ShipshieldBar) Platforms.Draw(window);
		for (Platform& Platforms : interface_Shipenchant) Platforms.Draw(window);
		for (Platform& Platforms : interface_Boosts) Platforms.Draw(window);
			//cout << count_Ultimate<<endl;
			window.draw(health_text);
			window.draw(mana_text);
			if (cooldown_Ultimate > 0 &&alive) window.draw(cd);
			window.draw(skillCost);
			if (cooldown_Shield > 0 && alive)window.draw(shield_cd);
			window.draw(shieldCost);
			if (cooldown_W > 0 && alive)window.draw(W_cd);
			window.draw(Wcost);
			if (cooldown_Q > 0 && alive)window.draw(Q_cd);
			window.draw(Qcost);
			window.draw(L);
			window.draw(ATK);
			window.draw(ATK_SPD);
			window.draw(MOVE);
			if (ship.info_ally.level < 2)
				window.draw(AleartW), window.draw(AleartW2);
			if (ship.info_ally.level < 4)
				window.draw(AleartE), window.draw(AleartE2);
			if (ship.info_ally.level < 6)
				window.draw(AleartR), window.draw(AleartR2);
			window.draw(SC);
			//changestage
			
			//if(!shields.empty())
			//cout << shieldOn << endl;
			//if(isBoosted)
			//cout << boosts[0].speed << endl;
			window.display();
			//cout << alive<<' '<<count_back<<endl;
			if (!alive && count_back > 2500)
			{
				ofstream file("./text/score.txt", std::ios::app);
				file << ' ';
				file << score;
				file << '\n';
				file.close();
				stage = 1;
				window.clear();
				bgm.play();
				alive = true;
				clock.restart();
				clock2.restart();
				clock3.restart();
				clock4.restart();
				clock5.restart();
				bossshottime.restart();
				Cooldown.restart();
				chargeTime.restart();
				gametime.restart();
				Cooldown_Shield.restart();
				shieldTime.restart();
				shieldStayTime.restart();
				Wtime.restart();
				Qtime.restart();
				boosttime.restart();
				bosstime.restart();
				back.restart();
				TB.cleartext();
				continue;
			}
			else if(alive)back.restart();
		}
		else if (stage == 6)
		{
		window.close();
		break;
		}
		else if (stage == 3)
		{
			//enter name and highscore
			cleardata();
			Event EVENT;
			/*ofstream f("./text/score.txt");
			if (!f)
			{
				ofstream fileTemp("./text/score.txt");
				fileTemp.close();
			}
			else f.close();*/
			//cout << "777" << endl;
			//std::cout << "Fuck!" << std::endl;
			while (window.pollEvent(ev)) {
				//std::cout << "poll event!" << std::endl;
				if (ev.type == Event::TextEntered)
				{
					if (Keyboard::isKeyPressed(Keyboard::Space)) {

						ofstream file("./text/score.txt", std::ios::app);
						
						if (file.is_open())
						{
							file << TB.getText();
							file.close();
						}
						
						cleardata();
						ship.info_ally.level = 1;
						ship.info_ally.expBar = 0.0f;
						ship.info_ally.levelGate = 350.0f;
						ship.info_ally.health = 1200.0f;
						ship.info_ally.mana = 620.0f;
						ship.body.setPosition(Vector2f(100.0f, 100.0f));
						back.restart();
						stage = 7;
						alive=true;
						bgm.pause();
						ingame.play();
						gameon = true;
						score = 0;
						break;
					}
					else {
						//std::cout << "do this" << std::endl;
						TB.typeOn(ev);
						if (Keyboard::isKeyPressed(Keyboard::BackSpace)) TB.Textbox.move(Vector2f(2, 0));
						else TB.Textbox.move(Vector2f(-2, 0));
					}
				}
			}
			Platform tempEn(&enterName, Vector2f(900, 550), Vector2f(-40, 0));
			window.clear();
			tempEn.Draw(window);
			TB.draw(window);
			window.display();
			/*ofstream MyWriteFile("./text/score.txt");
			MyWriteFile << Name;
			MyWriteFile.close();*/
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				stage = 1;
				continue;
			}
		}
		else if (stage == 4) {
		
		sf::Text _score;
		_score.setFont(font);
		_score.setCharacterSize(20);
		_score.setPosition(Vector2f(-30, -150));
		_score.setFillColor(Color::White);
		Platform temp(&Highscore_In, Vector2f(750, 550), Vector2f(0,20));
		window.clear();
		temp.Draw(window);
		sort(scorebar.begin()+1, scorebar.end() - 1, sortin);
		if (scorebar.size() > 8) {
			for (int i = 1; i < 6; i++) {
				_score.move(Vector2f(0, 40));
				_score.setString(scorebar[i].first + ' ' + str(scorebar[i].second));
				//	cout << scorebar[i].first<<' '<<i<<' '<< _score.getPosition().x << " : " << _score.getPosition().y << endl;
				window.draw(_score);
				//cout << scorebar.size() << endl;
			}
		}
		else {
			for (int i = 1; i < scorebar.size() - 2; i++) {
				_score.move(Vector2f(0, 40));
				_score.setString(scorebar[i].first + ' ' + str(scorebar[i].second));
				//	cout << scorebar[i].first<<' '<<i<<' '<< _score.getPosition().x << " : " << _score.getPosition().y << endl;
				window.draw(_score);
				//cout << scorebar.size() << endl;
			}
		}
		window.display();
		
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				stage = 1;
				continue;
		}

		}
		else if (stage == 5) {
			Platform temp(&Guide_In, Vector2f( 750,550), Vector2f(-40, 0));
			window.clear();
			temp.Draw(window);
			window.display();
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				stage = 1;
				continue;
			}
		}
		//cout << stage<<endl;
		//cout<< score << endl;
	}
	return 0;
}
void cleardata()
{
	while (!bullets.empty()) bullets.pop_back();
	while (!platforms.empty()) platforms.pop_back();
	while (!Levels.empty()) Levels.pop_back();
	while (!interface_ShipHealth.empty()) interface_ShipHealth.pop_back();
	while (!interface_ShipMana.empty()) interface_ShipMana.pop_back();
	while (!interface_ShipSkill.empty())interface_ShipSkill.pop_back();
	while (!interface_Shipenchant.empty()) interface_Shipenchant.pop_back();
	while (!interface_ShipshieldBar.empty()) interface_ShipshieldBar.pop_back();
	while (!interface_Boosts.empty()) interface_Boosts.pop_back();
	while (!enemybullets.empty()) enemybullets.pop_back();
	while (!enemys.empty()) enemys.pop_back();
	while (!bosss.empty()) bosss.pop_back();
	while (!skills.empty()) skills.pop_back();
	while (!shields.empty()) shields.pop_back();
	while (!boosts.empty()) boosts.pop_back();
	while (!players.empty()) players.pop_back();
	while (!charging.empty()) charging.pop_back();
	while (!shieldGone_animated.empty()) shieldGone_animated.pop_back();
	ultimateOn = false;
	shieldOn = false;
	isCharge = false;
	isShield = false;
	isEnchanted = false;
	isBoosted = false;
	alive = true;
	gametime.restart();
	currentlevel = 1;
	score = 0;
	
}

//