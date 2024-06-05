#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Timer.h"
#include "Map.h"
#include "MiniGameFishing.h"
#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Fishing.h"
#include "DialogTriggerEntity.h"
#include "DialogManager.h"
#include "Dialog.h"
#include "InventoryManager.h"
#include <map>
#include <random>

//base of the probability of catching fish
double nothing_probability = 0.4;
double trash_probability = 0.3;
double small_probability = 0.15;
double medium_probability = 0.1;
double big_probability = 0.05;

//probability machine for each type of fish
std::map<Fishlevel, double> fish = {
	{Fishlevel::NOTHING, nothing_probability},
	{Fishlevel::TRASH, trash_probability},
	{Fishlevel::SMALL, small_probability},
	{Fishlevel::MEDIUM, medium_probability},
	{Fishlevel::BIG, big_probability}
};

//probability machine for lure fishing
std::map<bool, double> isFishCaught = {
	{true, 0.3},
	{false, 0.7},
};


MiniGameFishing::MiniGameFishing() : Entity(EntityType::ROD)
{
	name = ("minigamefishing");

}

MiniGameFishing::~MiniGameFishing() {}

bool MiniGameFishing::Awake() {

	//texturePath = parameters.attribute("texturePath").as_string();


	return true;
}

bool MiniGameFishing::Start() {

	fishingfloat_path = parameters.child("fishingfloat").attribute("texturepath").as_string();
	fishingfloat_texture = app->tex->Load(fishingfloat_path);
	fishing.fishingtype = FISHINGTYPE::FISHING;
	floatChangeDistance = 300;


	for (pugi::xml_node node : parameters.child("fishlevel").children()) {
		std::vector<const char*> fishrow;
		std::vector<const char*> fishname;
		for (pugi::xml_node itemNode : node.children()) {
			const char* texturepath = itemNode.attribute("texturepath").value();
			fishrow.push_back(rutaPath(texturepath));
			texturepath = itemNode.attribute("name").value();
			fishname.push_back(rutaPath(texturepath));
		}
		chosefishing_path.push_back(fishrow);
		choseName_path.push_back(fishname);
	}
	/*for (const auto& vec :  chosefishing_path) {
		for (const auto& str : vec) {
			//printf("%s ", str);
		}
		//printf("\n");
	}*/
	return true;
}

bool MiniGameFishing::Update(float dt)
{
	//Automatic dialog box closure mechanism
	if (dialogoautoclose) {
		app->dialogManager->AutoNextDiagolo(dialogoTimeCount);
	}

	miniGameLoop(dt);
	miniGameEnd(dt);

	////printf("\nss: %s", fishingfloat_path);


	return true;
}

bool MiniGameFishing::PostUpdate()
{
	if (startAniFloat) {
		floatCollision(app->entityManager->GetPlayer()->player_Direction, cheke_x, cheke_y);
	}
	return true;
}


bool MiniGameFishing::CleanUp()
{
	if (floatbody != nullptr) {
		floatbody->body->GetWorld()->DestroyBody(floatbody->body);
		floatbody = nullptr;
	}
	app->tex->UnLoad(fishingfloat_texture);
	SDL_DestroyTexture(fishingfloat_texture);
	
	return true;
}

char* MiniGameFishing::rutaPath(const char* filePath)
{
	size_t path_length = strlen(filePath);
	char* texturepath_with_null = new char[path_length + 1];
	strcpy_s(texturepath_with_null, path_length + 1, filePath);
	texturepath_with_null[path_length] = '\0';
	return texturepath_with_null;
}

void MiniGameFishing::castingline(FISHINGTYPE type)
{
	if (fishing.rodReady) {
		if (fishing.isFishing) {
			fishingfloat_lineReady = true;
			crearfloatbody = true;
		}//start
		else {
			fishingfloat_lineReady = false;
			if (floatbody != nullptr) {
				app->physics->GetWorld()->DestroyBody(floatbody->body);
				floatbody = nullptr;
			}//end_if, delete collision
		}//end_if, if fishing
	}//end_if, check if equipped with a fishing rod
}

void MiniGameFishing::ani_castingline(Direction direction)
{
	//obtener position de player
	if (fishingfloat_getPlayerPosition) {
		fishingflota_position_x = app->entityManager->GetPlayer()->position.x;
		fishingflota_position_y = app->entityManager->GetPlayer()->position.y;
		fishingfloat_getPlayerPosition = false;
	}//end_if, can get player position

	floatDistance = floatChangeDistance;

	//Calculate the final position of the float
	if (direction == Direction::UP) { fishingflota_CenterX = app->entityManager->GetPlayer()->position.x; fishingflota_CenterY = app->entityManager->GetPlayer()->position.y - floatDistance; }
	else if (direction == Direction::DOWN) { fishingflota_CenterX = app->entityManager->GetPlayer()->position.x; fishingflota_CenterY = app->entityManager->GetPlayer()->position.y + floatDistance; }
	else if (direction == Direction::LEFT) { fishingflota_CenterX = app->entityManager->GetPlayer()->position.x - floatDistance; fishingflota_CenterY = app->entityManager->GetPlayer()->position.y; }
	else if (direction == Direction::RIGHT) { fishingflota_CenterX = app->entityManager->GetPlayer()->position.x + floatDistance; fishingflota_CenterY = app->entityManager->GetPlayer()->position.y; }
	else { fishingflota_CenterX = app->entityManager->GetPlayer()->position.x; fishingflota_CenterY = app->entityManager->GetPlayer()->position.y + floatDistance; }

	//crear float collision
	if (floatbody == nullptr && crearfloatbody) {
		floatbody = app->physics->CreateRectangleSensor(app->entityManager->GetPlayer()->position.x, app->entityManager->GetPlayer()->position.y, 20, 20, bodyType::DYNAMIC);
		floatbody->ctype = ColliderType::FLOAT;
		floatbody->listener = this;
		floatbody->entity = this;
		crearfloatbody = false;
	}

	
	//lerp
	float timeLerp = 0.1f;
	fishingflota_position_x = fishingflota_position_x * (1 - timeLerp) + fishingflota_CenterX * timeLerp;
	fishingflota_position_y = fishingflota_position_y * (1 - timeLerp) + fishingflota_CenterY * timeLerp;
	
	//moving collision
	if (floatbody != nullptr) {
		b2Transform pbodyPos = floatbody->body->GetTransform();
		cheke_x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
		cheke_y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	}
	startAniFloat = true;
}

void MiniGameFishing::playNormalFishing()
{
	if (timeFishing.ReadMSec() >= lotteryrandomNum * 1000) {
		if (thistimehooked) {
			thistimehooked = false;
			//Close dialogo
			dialogoClose(0);
			//Crear new dialogo
			app->dialogManager->CreateDialogSinEntity("Ostia, est?picando algo!", "Jakov", nullptr);
			playerGoplay = true;//Start game play
			gamePlayTime = getRandomNumber(3, 6);// Get play time
			gamePlayTimeLimit.Start();// reset play time
		}



		if (app->input->GetButton(FISHINGPLAY) == KEY_DOWN || app->input->GetButton(BACK) == KEY_DOWN) {
			player_click_count += 1;
		}//end_if, player count

		if (playerGoplay == true) {
			GamePlaye();
		}//end_if, gameplay


		if (playerGoplay_TimeOver && player_click_count_TimeOver == 0) {
			player_click_count_TimeOver = 0;//reset
			playerGoplay_TimeOver = false;
			//Close dialogo
			dialogoClose(0);
			//Crear new dialogo
			app->dialogManager->CreateDialogSinEntity("Qu?latima, el pez se ha escapado...", "Jakov", nullptr);
			fishingOver();
		}//end_if player no play, end fishing
	}//end_if, if fish is caught
}


void MiniGameFishing::floatCollision(Direction direction, float cheke_x, float cheke_y)
{
	
	b2Vec2 force(0.0f, 0.0f);

	if (direction == Direction::UP && cheke_y >= fishingflota_position_y) {
		force.y = -10.0f;
	}
	else if (direction == Direction::DOWN && cheke_y <= fishingflota_position_y) {
		force.y = 10.0f;
	}
	else if (direction == Direction::LEFT && cheke_x >= fishingflota_position_x) {
		force.x = -10.0f;
	}
	else if (direction == Direction::RIGHT && cheke_x <= fishingflota_position_x) {
		force.x = 10.0f;
	}
	else if (direction == Direction::UNKNOWN && cheke_y <= fishingflota_position_y) {
		force.y = 10.0f;
	}


	if (force == b2Vec2(0.0f, 0.0f)) {
		
		if (floatbody != nullptr) {
			floatbody->body->GetWorld()->DestroyBody(floatbody->body);
			floatbody = nullptr;
		}//end_if, delete collision
		if (floatbody != nullptr) {
			floatbody->body->SetLinearVelocity(b2Vec2(0, 0));
		}
	}
	else {
		if (floatbody != nullptr) {
		floatbody->body->ApplyForceToCenter(force, true);
		}
	}

	if (direction == Direction::LEFT || direction == Direction::RIGHT) {
		app->render->DrawTexture(fishingfloat_texture, fishingflota_position_x, fishingflota_position_y - 23, 3);
	}
	else {
		app->render->DrawTexture(fishingfloat_texture, fishingflota_position_x - 23, fishingflota_position_y, 3);
	}
}

void MiniGameFishing::dialogoClose(int time)
{
	//Close dialogo
	dialogoTimeCount = time;
	dialogoautoclose = true;
	app->dialogManager->autoNextTime_TimerDown.Start();
}

void MiniGameFishing::hooked(int player_click_count)
{

	if (player_click_count < 10) {
		changeProbability(0.4, 0.3, 0.15, 0.1, 0.05);
	}
	else if (player_click_count >= 10 && player_click_count < 20) {
		changeProbability(0.3, 0.4, 0.15, 0.1, 0.05);
	}
	else if (player_click_count >= 20 && player_click_count < 30) {
		changeProbability(0.25, 0.35, 0.2, 0.15, 0.05);
	}
	else if (player_click_count >= 30 && player_click_count < 40) {

		changeProbability(0.2, 0.35, 0.2, 0.15, 0.1);
	}
	else if (player_click_count >= 40 && player_click_count < 50) {

		changeProbability(0.1, 0.25, 0.3, 0.2, 0.15);
	}
	else if (player_click_count > 50) {
		changeProbability(0.0, 0.0, 0.0, 0.0, 1.0);
	}//end_if, use different probabilities based on the player

	//lottery machine
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0.0, 1.0);
	double random_number = dis(gen);

	double cumulative_probability = 0.0;
	selected_fish;
	for (const auto& check : fish) {
		cumulative_probability += check.second;
		if (random_number <= cumulative_probability) {
			selected_fish = check.first;
			break;
		}
	}

	for (const auto& prize : fish) {
		//printf("\n%f", prize.second);
	}// print probabilities


	reward_pool(selected_fish);


}

void MiniGameFishing::GamePlaye()
{
	gamePlayTimeLimit_show = gamePlayTimeLimit.CountDown(gamePlayTime);

	if ((float)gamePlayTimeLimit_show == 0) {
		//printf("\nTimeStop, you get click %d times", player_click_count);
		playerGoplay = false;
		playerGoplay_TimeOver = true;
		player_click_count_TimeOver = player_click_count;
		if (player_click_count_TimeOver != 0) {
			hooked(player_click_count);
		}//end_if, if player click not 0
		player_click_count = 0;
	}//end_if, if gameplaytime over
}

void MiniGameFishing::resetProbability() {
	//if the probability is changed, it will reset here.
	nothing_probability = 0.4;
	trash_probability = 0.3;
	small_probability = 0.15;
	medium_probability = 0.1;
	big_probability = 0.05;

	fish[Fishlevel::NOTHING] = nothing_probability;
	fish[Fishlevel::TRASH] = trash_probability;
	fish[Fishlevel::SMALL] = small_probability;
	fish[Fishlevel::MEDIUM] = medium_probability;
	fish[Fishlevel::BIG] = big_probability;

}

void MiniGameFishing::changeProbability(double nothing, double trash, double small, double medium, double big)
{
	//The probability is changed here
	nothing_probability = nothing;
	trash_probability = trash;
	small_probability = small;
	medium_probability = medium;
	big_probability = big;

	fish[Fishlevel::NOTHING] = nothing_probability;
	fish[Fishlevel::TRASH] = trash_probability;
	fish[Fishlevel::SMALL] = small_probability;
	fish[Fishlevel::MEDIUM] = medium_probability;
	fish[Fishlevel::BIG] = big_probability;
}

int MiniGameFishing::getRandomNumber(int min, int max) {
	//random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

bool MiniGameFishing::check_isFishCaught()
{
	//Lure Fishing, whether the lottery results in catching a fish
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0.0, 1.0);
	double random_number = dis(gen);

	double cumulative_probability = 0.0;

	for (const auto& cheke : isFishCaught) {
		cumulative_probability += cheke.second;
		if (random_number <= cumulative_probability) {
			return cheke.first;
			break;
		}
	}
}

void MiniGameFishing::fishingOver()
{
	//if fishing ends
	if (app->entityManager->GetPlayer()->playermove == false && lureFinishLine == false) {
		fishingEndCloseDialogo = true;
	}//end_if, close dialogo if is last

	lureFinishLine = false;
	fishing.isFishing = false;
	fishing.startFishing = false;
	if (!fishing.isFishing) {
		fishingfloat_getPlayerPosition = true;
	}//end if, if not fishing
	startFinishingLine = false;
	startAniFloat = false;
	castingline(fishingtype);
}

void MiniGameFishing::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::LAGO:

		if (fishing.startFishing == false) {
			 app->dialogManager->CreateDialogSinEntity("A ver si pican...", "Jakov", nullptr);
		}//end_if, if the float collides with the " LAGO"collider, display the fishing dialog
		fishing.startFishing = true;
		dialogoPlayerMoving = true;
		timeFishing.Start();

		if (fishing.fishingtype == FISHINGTYPE::FISHING) {
			lotteryrandomNum = getRandomNumber(2, 4);
		}
		thistimehooked = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}


bool MiniGameFishing::miniGameLoop(float dt)
{
	//Cast the rod and StartFishing
	if (app->input->GetButton(STARTFISHING) == KEY_DOWN && fishing.rodReady || app->input->GetButton(DASH) == KEY_DOWN && fishing.rodReady) {
		fishing.isFishing = !fishing.isFishing;//Start fishing o Stop fishing
		
		isFishingta = fishing.isFishing;//Save data to "isFishingta"
		floatChangeDistance = 300;

		//Retrieve the rod
		if (!fishing.isFishing) {
			//Close dialogo
			dialogoClose(0);
			dialogoPlayerMoving = false;
			fishingfloat_getPlayerPosition = true;//Enable acquiring player's current location
			lureFinishLine = true;//In order to avoid repetition between the closing dialog for reeling in and the automatic dialog
			fishingOver();
		}//end_if for if not fishing

		castingline(fishing.fishingtype);//check if can fishing
	}//end_if for press "F" key

	//Animation float
	if (fishingfloat_lineReady) {
		ani_castingline(app->entityManager->GetPlayer()->player_Direction);//animation and collision of the float
	}//end_if can fishing or not

	//GamePlaye
	if (fishing.startFishing) {
		playNormalFishing();
	}//end_if, if start fishing
	return true;
}

bool MiniGameFishing::miniGameEnd(float dt)
{

	//if player mover, end fishing
	if (app->entityManager->GetPlayer()->playermove) {
		if (dialogoPlayerMoving) {
			//Close dialogo
			dialogoClose(0);
			dialogoPlayerMoving = false;
		}//end_if, if player fishing moving, close dialogo
		fishingOver();
	}//end_if player moving in fishing

	if (isEnd) {
		dialogoClose(2);
		fishingEndCloseDialogo = false;
		isEnd = false;
	}//end_if, close last dialogo

	return true;
}

void MiniGameFishing::reward_pool(Fishlevel fishingType)
{
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}
	std::string descripcionBasura;
	descripcionBasura = "basura";
	std::string descripcionPequeno;
	descripcionPequeno = "Un pez pequeño";
	std::string descripcionMediano;
	descripcionMediano = "Un pez mediano";
	std::string descripcionGrande;
	descripcionGrande = "Un pez grande";
	std::string tipo;
	tipo = "Pez";
	switch (fishingType)
	{
	case Fishlevel::NOTHING: fishLevel = 0; break;
	case Fishlevel::TRASH: 
		fishLevel = 1;
		if (app->inventoryManager->IsFull() == false)
		{
			app->inventoryManager->CreateItem(InventityType::BASURA, true);

		}
		else
		{
			Entity* entity = new Entity(EntityType::ITEM_BASURA);
			app->inventoryManager->AddQuantity(entity);
		}
		break;
		break;
	case Fishlevel::SMALL: 
		fishLevel = 2;
		if (app->inventoryManager->IsFull() == false)
		{
			app->inventoryManager->CreateItem(InventityType::PEZ_PEQUENO, true);


		}
		else
		{
			Entity* entity = new Entity(EntityType::ITEM_PEZ_PEQUENO);
			app->inventoryManager->AddQuantity(entity);
		}
		break;
	case Fishlevel::MEDIUM: 
		fishLevel = 3;
		if (app->inventoryManager->IsFull() == false)
		{
			app->inventoryManager->CreateItem(InventityType::PEZ_MEDIANO, true);
		}
		else
		{
			Entity* entity = new Entity(EntityType::ITEM_PEZ_MEDIANO);
			app->inventoryManager->AddQuantity(entity);
		}
		
		break;
	case Fishlevel::BIG: 
		fishLevel = 4; 
		if (app->inventoryManager->IsFull() == false)
		{
			app->inventoryManager->CreateItem(InventityType::PEZ_GRANDE, true);
		}
		else
		{
			Entity* entity = new Entity(EntityType::ITEM_PEZ_GRANDE);
			app->inventoryManager->AddQuantity(entity);
		}
		
		break;
	case Fishlevel::UNKNOWN:LOG("Collision UNKNOWN"); break;
	}//Reaction upon knowing what is obtained

	int num = 0;
	num = getRandomNumber(0, chosefishing_path[fishLevel].size() - 1);
	fishing_path = chosefishing_path[fishLevel][num];
	name_path = choseName_path[fishLevel][num];
	std::string strNumber = std::to_string(player_click_count);
	std::string frase = "He conseguido: ";
	dialogoClose(0);
	//app->dialogManager->CreateDialogSinEntity("you click " + strNumber + " veces " + " tu obtenido " + name_path, "Jakov", fishing_path);
	app->dialogManager->CreateDialogSinEntity(frase + name_path, "Jakov", fishing_path);
	//app->dialogManager->CreateDialogSinEntity("you click " + strNumber + " veces " + " tu obtenido " , "Fishing System", nullptr);
	fishingOver();
	resetProbability();
}


