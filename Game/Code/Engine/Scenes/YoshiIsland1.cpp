#include "YoshiIsland1.h"

#include "../../GameObjects/Bill.h"
#include "../../GameObjects/Blocks.h"
#include "../../GameObjects/Collectables.h"
#include "../../GameObjects/Chuck.h"
#include "../../GameObjects/Koopa.h"
#include "../../GameObjects/Pipes.h"
#include "../../GameObjects/PPlant.h"
#include "../../GameObjects/Rex.h"
#include <Drawables/SFSprite.h>
#include <Drawables/SFText.h>
#include <Renderer/SFCamera.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Collisions/ITile.h>
#include <Utilities/Utils.h>
#include <SFML/Graphics/View.hpp>

YoshiIsland1::YoshiIsland1()
{
	m_backgroundSpr = std::make_shared<SFSprite>("Background");
	ENSURE_VALID(m_backgroundSpr);

	DECL_GET_OR_RETURN(bkgSpr,dynamic_cast<SFSprite*>(m_backgroundSpr.get()));

	bkgSpr->SetScale(GameConstants::Scale);
	bkgSpr->SetOrigin(Vector2f());
	bkgSpr->SetPosition(Vector2f(0.f, -480.f));
}

void YoshiIsland1::SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos)
{
	ENSURE_VALID(obj);

	obj->SetPosition(pos);

	if (m_objects.find(id) != m_objects.end())
		THROW_IF_FALSE_MSG(false, "Key '{}' already exists", id);

	auto [it, inserted] = m_objects.emplace(id, std::move(obj));
	THROW_IF_FALSE_MSG(inserted, "Key '{}' already exists", id);
	m_spawnedObjKeys.push_back(it->first);
}

bool YoshiIsland1::AddEnemies()
{
	DECL_GET_ENSURE_VALID_RET(gameMgr, GameManager::Get(), false);
	DECL_GET_ENSURE_VALID_RET(colMgr, gameMgr->GetCollisionMgr(), false);

	ITile* tmp;

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(13, 5), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Koopa>("Koopa1", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(31, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Bill>("Bill1", false, tmp->GetPosition() + Vector2f(0, 22));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(37, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex1", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(47, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex2", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(55, 6), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex3", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(72, 6), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex4", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(82, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex5", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(89, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex6", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(112, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex7", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(113, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<PPlant>("PPlant1", tmp->GetPosition() + Vector2f(18, 16));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(138, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex8", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(139, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<PPlant>("PPlant2", tmp->GetPosition() + Vector2f(18, 16));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(172, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex9", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(182, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex10", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(184, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex11", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(186, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex12", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(189, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex13", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(203, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Bill>("Bill2", false, tmp->GetPosition() + Vector2f(0, 20));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(207, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex14", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(227, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Bill>("Bill3", false, tmp->GetPosition() + Vector2f(0, 20));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(242, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex15", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(257, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex16", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(263, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex17", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(280, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Bill>("Bill4", false, tmp->GetPosition() + Vector2f(0, 20));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(284, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<PPlant>("PPlant3", tmp->GetPosition() + Vector2f(18, 16));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(290, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Rex>("Rex18", false, tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(298, 10), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceEnemyOrThrow<Chuck>("Chuck1", false, tmp->GetPosition() + Vector2f(0, 24));

	return true;
}

bool YoshiIsland1::AddGUI()
{
	DECL_GET_ENSURE_VALID_RET(gameMgr, GameManager::Get(), false);

	EmplaceGUISpriteOrThrow<SFSprite>("Name", "Name");
	EmplaceGUISpriteOrThrow<SFSprite>("Time", "Time");

	for (auto& [_, spr] : m_sprites)
	{
		CONTINUE_IF_INVALID(spr);

		GET_OR_CONTINUE(sfSpr, dynamic_cast<SFSprite*>(spr.get()));

		sfSpr->SetScale({ 1.f,1.f });
	}

	TextConfig config;
	config.m_alignment = TextAlignment::None;
	config.m_charSize = 15;
	config.m_fontName = "Arial";
	config.m_colour = Colour::Black;

	EmplaceGUITextOrThrow<SFText>("Name", config);
	EmplaceGUITextOrThrow<SFText>("Time", config);

	for (auto& [_, txt] : m_texts)
	{
		CONTINUE_IF_INVALID(txt);

		txt->SetFillColour(Colour::Yellow);
	}

	GetGUITextByName("Name")->SetText("x 00");
	GetGUITextByName("Time")->SetText(std::to_string(static_cast<int>(gameMgr->GetTimer().GetCurrTime())));

	return true;
}

bool YoshiIsland1::AddObjects()
{
	DECL_GET_ENSURE_VALID_RET(gameMgr, GameManager::Get(), false);
	DECL_GET_ENSURE_VALID_RET(colMgr, gameMgr->GetCollisionMgr(), false);

	ITile* tmp;

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(17, 4), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<YCoin>("YCoin1", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(37, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<QuestionBlock>("QBox1", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(89, 5), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<YCoin>("YCoin2", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(102, 11), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<Mushroom>("Shroom1", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(120, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<RotatingBlock>("RBox1", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(121, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<RotatingBlock>("RBox2", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(151, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<CheckPoint>("ChkPnt", tmp->GetPosition() - Vector2f(3, 10.5f));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(179, 2), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<YCoin>("YCoin3", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(239, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<RotatingBlock>("RBox3", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(243, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<RotatingBlock>("RBox4", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(247, 8), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<RotatingBlock>("RBox5", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(243, 5), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<QuestionBlock>("QBox2", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(288, 4), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<YCoin>("YCoin4", tmp->GetPosition());

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(302, 3), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<Goal>("Goal", Vector2f(tmp->GetPosition().x - 7.f, tmp->GetPosition().y + 16));

	return true;
}

bool YoshiIsland1::AddForeGroundObjects()
{
	DECL_GET_ENSURE_VALID_RET(gameMgr, GameManager::Get(), false);
	DECL_GET_ENSURE_VALID_RET(colMgr, gameMgr->GetCollisionMgr(), false);

	ITile* tmp;

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(113, 10), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<Pipe>("Pipe1", "Pipe1", tmp->GetPosition() + Vector2f(18.5f, 0.f));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(140, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<Pipe>("Pipe2", "Pipe2", tmp->GetPosition() + Vector2f(18.5f, 0.f));

	ERR_TRY_ASSIGN_OR_RET(tmp, colMgr->GetTile(284, 9), false);
	ENSURE_VALID_RET(tmp, false);
	EmplaceObjectOrThrow<Pipe>("Pipe3", "Pipe3", tmp->GetPosition() + Vector2f(18.5f, 0.f));

	return true;
}

void YoshiIsland1::UpdateGUI(float deltaTime)
{
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(cam, gameMgr->GetCamera());
	DECL_GET_OR_RETURN(sfCam, dynamic_cast<SFCamera*>(cam));
	DECL_GET_OR_RETURN(view, sfCam->GetView());

	DECL_GET_OR_RETURN(spr, dynamic_cast<SFSprite*>(GetGUISpriteByName("Name")));
	DECL_GET_OR_RETURN(txt, dynamic_cast<SFText*>(GetGUITextByName("Name")));

	spr->SetPosition(Vector2f((view->getCenter().x - GameConstants::ScreenDim.x * 0.5f) + 30, 20));
	txt->SetPosition(spr->GetPosition() + Vector2f(static_cast<float>(spr->GetTextureSize().x) * 0.5f + 20, -10));

	GET_OR_RETURN(spr, dynamic_cast<SFSprite*>(GetGUISpriteByName("Time")));
	GET_OR_RETURN(txt, dynamic_cast<SFText*>(GetGUITextByName("Time")));

	spr->SetPosition(Vector2f(view->getCenter().x, 20));
	txt->SetPosition(spr->GetPosition() + Vector2f(static_cast<float>(spr->GetTextureSize().x) * 0.5f + 20, -10));
	txt->SetText(std::to_string(static_cast<int>(gameMgr->GetTimer().GetCurrTime())));
}