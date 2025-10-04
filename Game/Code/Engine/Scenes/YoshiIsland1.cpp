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
#include <SFML/Graphics/View.hpp>

YoshiIsland1::YoshiIsland1()
{
	m_backgroundSpr = std::make_shared<SFSprite>("Background");

	auto bkgSpr = dynamic_cast<SFSprite*>(m_backgroundSpr.get());
	if (bkgSpr)
	{
		bkgSpr->SetScale(GameConstants::Scale);
		bkgSpr->SetOrigin(GameConstants::ScreenDim / 2.f);
		bkgSpr->SetPosition(GameConstants::ScreenDim / 2.f);
	}
}

void YoshiIsland1::SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos)
{
	if (!obj)
		return;

	obj->SetPosition(pos);

	auto [it, inserted] = m_objects.emplace(id, std::move(obj));
	if (inserted)
		m_spawnedObjKeys.push_back(it->first);
}

void YoshiIsland1::AddEnemies()
{
	auto colMgr = GameManager::Get()->GetCollisionMgr();

	ITile* tmp;

	if (tmp = colMgr->GetTile(13, 5))
		m_enemies.emplace("Koopa1", std::make_shared<Koopa>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(31, 8))
		m_enemies.emplace("Bill1", std::make_shared<Bill>(false, tmp->GetPosition() + Vector2f(0, 22)));

	if (tmp = colMgr->GetTile(37, 11))
		m_enemies.emplace("Rex1", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(47, 11))
		m_enemies.emplace("Rex2", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(55, 6))
		m_enemies.emplace("Rex3", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(72, 6))
		m_enemies.emplace("Rex4", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(82, 11))
		m_enemies.emplace("Rex5", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(89, 11))
		m_enemies.emplace("Rex6", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(112, 11))
		m_enemies.emplace("Rex7", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(113, 9))
		m_enemies.emplace("PPlant1", std::make_shared<PPlant>(tmp->GetPosition() + Vector2f(18, 16)));

	if (tmp = colMgr->GetTile(138, 11))
		m_enemies.emplace("Rex8", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(139, 8))
		m_enemies.emplace("PPlant2", std::make_shared<PPlant>(tmp->GetPosition() + Vector2f(18, 16)));

	if (tmp = colMgr->GetTile(172, 11))
		m_enemies.emplace("Rex9", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(182, 11))
		m_enemies.emplace("Rex10", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(184, 11))
		m_enemies.emplace("Rex11", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(186, 11))
		m_enemies.emplace("Rex12", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(189, 11))
		m_enemies.emplace("Rex13", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(203, 9))
		m_enemies.emplace("Bill2", std::make_shared<Bill>(false, tmp->GetPosition() + Vector2f(0, 20)));

	if (tmp = colMgr->GetTile(207, 11))
		m_enemies.emplace("Rex14", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(227, 9))
		m_enemies.emplace("Bill3", std::make_shared<Bill>(false, tmp->GetPosition() + Vector2f(0, 20)));

	if (tmp = colMgr->GetTile(242, 11))
		m_enemies.emplace("Rex15", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(257, 11))
		m_enemies.emplace("Rex16", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(263, 11))
		m_enemies.emplace("Rex17", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(280, 9))
		m_enemies.emplace("Bill3", std::make_shared<Bill>(false, tmp->GetPosition() + Vector2f(0, 20)));

	if (tmp = colMgr->GetTile(284, 8))
		m_enemies.emplace("PPlant3", std::make_shared<PPlant>(tmp->GetPosition() + Vector2f(18, 16)));

	if (tmp = colMgr->GetTile(290, 8))
		m_enemies.emplace("Rex17", std::make_shared<Rex>(false, tmp->GetPosition()));

	if (tmp = colMgr->GetTile(298, 10))
		m_enemies.emplace("Chuck1", std::make_shared<Chuck>(false, tmp->GetPosition() + Vector2f(0, 24)));
}

void YoshiIsland1::AddGUI()
{
	m_sprites.push_back(std::make_shared<SFSprite>("Name"));
	m_sprites.push_back(std::make_shared<SFSprite>("Time"));

	TextConfig config;
	config.m_alignment = TextAlignment::None;
	config.m_charSize = 15;
	config.m_fontName = "Arial";
	config.m_colour = Colour::Black;

	m_texts.push_back(std::make_shared<SFText>(config));
	m_texts.push_back(std::make_shared<SFText>(config));

	for (auto txt : m_texts)
		txt->SetFillColour(Colour::Yellow);

	m_texts[static_cast<int>(Texts::Name)]->SetText("x 00");
	m_texts[static_cast<int>(Texts::Time)]->SetText(std::to_string(static_cast<int>(GameManager::Get()->GetTimer().GetCurrTime())));
}

void YoshiIsland1::AddObjects()
{
	auto colMgr = GameManager::Get()->GetCollisionMgr();

	ITile* tmp;

	if (tmp = colMgr->GetTile(17, 4))
		m_objects.emplace("YCoin1", std::make_shared<YCoin>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(37, 8))
		m_objects.emplace("QBox1", std::make_shared<QuestionBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(89, 5))
		m_objects.emplace("YCoin2", std::make_shared<YCoin>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(102, 11))
		m_objects.emplace("Shroom1", std::make_shared<Mushroom>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(120, 8))
		m_objects.emplace("RBox1", std::make_shared<RotatingBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(121, 8))
		m_objects.emplace("RBox2", std::make_shared<RotatingBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(151, 9))
		m_objects.emplace("ChkPnt", std::make_shared<CheckPoint>(tmp->GetPosition() - Vector2f(3, 10.5f)));

	if (tmp = colMgr->GetTile(179, 2))
		m_objects.emplace("YCoin3", std::make_shared<YCoin>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(239, 8))
		m_objects.emplace("RBox3", std::make_shared<RotatingBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(243, 9))
		m_objects.emplace("RBox4", std::make_shared<RotatingBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(247, 8))
		m_objects.emplace("RBox5", std::make_shared<RotatingBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(243, 5))
		m_objects.emplace("QBox2", std::make_shared<QuestionBlock>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(288, 4))
		m_objects.emplace("YCoin4", std::make_shared<YCoin>(tmp->GetPosition()));

	if (tmp = colMgr->GetTile(302, 3))
		m_objects.emplace("Goal", std::make_shared<Goal>(Vector2f(tmp->GetPosition().x - 7.f, tmp->GetPosition().y + 16)));
}

void YoshiIsland1::AddForeGroundObjects()
{
	auto colMgr = GameManager::Get()->GetCollisionMgr();

	ITile* tmp;

	if (tmp = colMgr->GetTile(113, 10))
		m_objects.emplace("Pipe1", std::make_shared<Pipe>("Pipe1", tmp->GetPosition() + Vector2f(18.5f, 0.f)));

	if (tmp = colMgr->GetTile(140, 9))
		m_objects.emplace("Pipe2", std::make_shared<Pipe>("Pipe2", tmp->GetPosition() + Vector2f(18.5f, 0.f)));

	if (tmp = colMgr->GetTile(284, 9))
		m_objects.emplace("Pipe3", std::make_shared<Pipe>("Pipe3", tmp->GetPosition() + Vector2f(18.5f, 0.f)));
}

void YoshiIsland1::UpdateGUI(float deltaTime)
{
	auto sfCam = dynamic_cast<SFCamera*>(GameManager::Get()->GetCamera());
	if (!sfCam)
		return;

	auto view = sfCam->GetView();
	if (!view)
		return;

	auto spr = dynamic_cast<SFSprite*>(m_sprites[static_cast<int>(Sprites::Name)].get());
	auto txt = dynamic_cast<SFText*>(m_texts[static_cast<int>(Texts::Name)].get());
	if (spr && txt)
	{
		spr->SetPosition(Vector2f((view->getCenter().x - GameConstants::ScreenDim.x * 0.5f) + 30, 20));
		txt->SetPosition(spr->GetPosition() + Vector2f(static_cast<float>(spr->GetTextureSize().x) * 0.5f + 20, -10));
	}

	spr = dynamic_cast<SFSprite*>(m_sprites[static_cast<int>(Sprites::Time)].get());
	txt = dynamic_cast<SFText*>(m_texts[static_cast<int>(Texts::Time)].get());
	if (spr && txt)
	{
		spr->SetPosition(Vector2f(view->getCenter().x, 20));
		txt->SetPosition(spr->GetPosition() + Vector2f(static_cast<float>(spr->GetTextureSize().x) * 0.5f + 20, -10));
		txt->SetText(std::to_string(static_cast<int>(GameManager::Get()->GetTimer().GetCurrTime())));
	}
}