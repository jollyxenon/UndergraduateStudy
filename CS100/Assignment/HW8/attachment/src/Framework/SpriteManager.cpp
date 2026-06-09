#include "pvz/Framework/SpriteManager.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SOIL/SOIL.h>

#include "pvz/utils.hpp"
#include <iostream>

SpriteManager::SpriteManager() : m_spriteInfos() {
	const std::string redRepeaterAsset =
		std::filesystem::exists(std::filesystem::path(ASSET_DIR) / "red_repeater_spritesheet.png")
			? "red_repeater_spritesheet.png"
			: "repeater_spritesheet.png";
	const std::string redPeaAsset =
		std::filesystem::exists(std::filesystem::path(ASSET_DIR) / "red_pea.png")
			? "red_pea.png"
			: "pea.png";

	m_spriteInfos.insert({ EncodeAnim(ImageID::BACKGROUND, AnimID::NO_ANIMATION), SpriteInfo{"background.png", 800, 600, 800, 600} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::SUN, AnimID::IDLE), SpriteInfo{"sun_spritesheet.png", 512, 512, 80, 80, 6, 12} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::SHOVEL, AnimID::NO_ANIMATION), SpriteInfo{"shovel.png", 80, 80, 80, 80} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::COOLDOWN_MASK, AnimID::NO_ANIMATION), SpriteInfo{"seedpacket_cooldown.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::BRAIN_ICON, AnimID::NO_ANIMATION), SpriteInfo{"brain_icon.png", 32, 31, 32, 31} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::RED_LINE, AnimID::NO_ANIMATION), SpriteInfo{"red_line.png", 22, 502, 22, 502} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_EMPTY, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_empty.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_FULL, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_full.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_FILL, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_progress.png", 86, 11, 86, 11} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_1, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_1.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_2, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_2.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_3, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_3.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_4, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_4.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_5, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_5.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_6, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_6.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_7, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_7.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PROGRESS_METER_STAGE_8, AnimID::NO_ANIMATION), SpriteInfo{"flag_meter_stage_8.png", 158, 24, 158, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::ZOMBIE_CARD_REGULAR, AnimID::NO_ANIMATION), SpriteInfo{"zombie_card_regular.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::ZOMBIE_CARD_CONEHEAD, AnimID::NO_ANIMATION), SpriteInfo{"zombie_card_conehead.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::ZOMBIE_CARD_POLE, AnimID::NO_ANIMATION), SpriteInfo{"zombie_card_pole.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::ZOMBIE_CARD_BUCKET, AnimID::NO_ANIMATION), SpriteInfo{"zombie_card_bucket.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::ZOMBIE_CARD_BUNGEE, AnimID::NO_ANIMATION), SpriteInfo{"zombie_card_bungee.png", 50, 70, 50, 70} });

	m_spriteInfos.insert({ EncodeAnim(ImageID::SEED_SUNFLOWER, AnimID::NO_ANIMATION), SpriteInfo{"seedpacket_sunflower.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::SEED_PEASHOOTER, AnimID::NO_ANIMATION), SpriteInfo{"seedpacket_peashooter.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::SEED_WALLNUT, AnimID::NO_ANIMATION), SpriteInfo{"seedpacket_wallnut.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::SEED_REPEATER, AnimID::NO_ANIMATION), SpriteInfo{"seedpacket_repeater.png", 50, 70, 50, 70} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::SEED_RED_REPEATER, AnimID::NO_ANIMATION), SpriteInfo{"seedpacket_red_repeater.png", 50, 70, 50, 70} });

	m_spriteInfos.insert({ EncodeAnim(ImageID::SUNFLOWER, AnimID::IDLE), SpriteInfo{"sunflower_spritesheet.png", 512, 512, 80, 80, 6, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::PEASHOOTER, AnimID::IDLE), SpriteInfo{"peashooter_spritesheet.png", 512, 512, 80, 80, 6, 24} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::WALLNUT, AnimID::IDLE), SpriteInfo{"wallnut_spritesheet.png", 512, 512, 80, 80, 6, 32} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::REPEATER, AnimID::IDLE), SpriteInfo("repeater_spritesheet.png", 512, 512, 80, 80, 6, 24) });
	m_spriteInfos.insert({ EncodeAnim(ImageID::WALLNUT_CRACKED, AnimID::IDLE), SpriteInfo{"wallnut_cracked_spritesheet.png", 512, 512, 80, 80, 6, 32} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::RED_REPEATER, AnimID::IDLE), SpriteInfo(redRepeaterAsset, 512, 512, 80, 80, 6, 24) });

	m_spriteInfos.insert({ EncodeAnim(ImageID::REGULAR_ZOMBIE, AnimID::WALK), SpriteInfo{"zombie_walk_spritesheet.png", 1024, 1024, 100, 139, 10, 46} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::REGULAR_ZOMBIE, AnimID::EAT), SpriteInfo{"zombie_eat_spritesheet.png", 1024, 1024, 100, 139, 10, 39} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::CONEHEAD_ZOMBIE, AnimID::WALK), SpriteInfo{"conehead_zombie_walk_spritesheet.png", 1024, 1024, 100, 139, 10, 47} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::CONEHEAD_ZOMBIE, AnimID::EAT), SpriteInfo{"conehead_zombie_eat_spritesheet.png", 1024, 1024, 100, 139, 10, 40} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::BUCKET_HEAD_ZOMBIE, AnimID::WALK), SpriteInfo{"bucket_head_walk_spritesheet.png", 1024, 1024, 100, 139, 10, 46} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::BUCKET_HEAD_ZOMBIE, AnimID::EAT), SpriteInfo{"bucket_head_eat_spritesheet.png", 1024, 1024, 100, 139, 10, 39} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::POLE_VAULTING_ZOMBIE, AnimID::WALK), SpriteInfo{"pole_vaulter_walk_spritesheet.png", 1024, 1024, 100, 180, 10, 44} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::POLE_VAULTING_ZOMBIE, AnimID::EAT), SpriteInfo{"pole_vaulter_eat_spritesheet.png", 1024, 1024, 100, 180, 10, 27} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::POLE_VAULTING_ZOMBIE, AnimID::RUN), SpriteInfo{"pole_vaulter_run_spritesheet.png", 2048, 2048, 300, 180, 6, 36} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::POLE_VAULTING_ZOMBIE, AnimID::JUMP), SpriteInfo{"pole_vaulter_jump_spritesheet.png", 2048, 2048, 500, 180, 4, 42} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::BUNGEE_ZOMBIE, AnimID::NO_ANIMATION), SpriteInfo{"bungee_zombie_spritesheet.png", 102, 90, 102, 90} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::BUNGEE_ZOMBIE_GRAB, AnimID::NO_ANIMATION), SpriteInfo{"bungee_zombie_grab_spritesheet.png", 408, 90, 102, 90, 4, 4} });

	m_spriteInfos.insert({ EncodeAnim(ImageID::PEA, AnimID::NO_ANIMATION), SpriteInfo{"pea.png", 28, 28, 28, 28} });
	m_spriteInfos.insert({ EncodeAnim(ImageID::RED_PEA, AnimID::NO_ANIMATION), SpriteInfo{redPeaAsset, 28, 28, 28, 28} });

	m_spriteInfos.insert({ EncodeAnim(ImageID::ZOMBIES_WON, AnimID::NO_ANIMATION), SpriteInfo{"ZombiesWon.jpg", 564, 468, 564, 468} });

	LoadSprites();
}

bool SpriteManager::LoadSprites(){
	glEnable(GL_DEPTH_TEST);
	for (auto& asset : m_spriteInfos) {


		GLuint texture = SOIL_load_OGL_texture(asset.second.filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
																					 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		if (0 == texture) {
			std::cout << "[ERROR] Error loading asset " << asset.second.filename
				<< ": " << SOIL_last_result() << std::endl;
		}

		asset.second.texture = texture;
	}
	return true;
}

SpriteInfo SpriteManager::GetSpriteInfo(ImageID imageID, AnimID animID) const {
	auto it = m_spriteInfos.find(EncodeAnim(imageID, animID));
	if (it == m_spriteInfos.end()) {
		return SpriteInfo("", 0, 0, 0, 0);
	}
	else {
		return it->second;
	}
}


