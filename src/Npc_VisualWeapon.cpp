/*
** Made by Rochet2(Eluna)
** Rewritten by Poszer & Talamortis https://github.com/poszer/ & https://github.com/talamortis/
** AzerothCore 2019 http://www.azerothcore.org/
** Cleaned and made into a module by Micrah https://github.com/milestorme/
*/

/*
    --Import this SQL first:
    CREATE TABLE `custom_item_enchant_visuals` (
        `iguid` INT(10) UNSIGNED NOT NULL COMMENT 'item DB guid',
        `display` INT(10) UNSIGNED NOT NULL COMMENT 'enchantID',
		`PlayerName` varchar(50) NOT NULL,
        PRIMARY KEY(`iguid`)
        )
        COMMENT = 'stores the enchant IDs for the visuals'
        COLLATE = 'latin1_swedish_ci'
        ENGINE = InnoDB;
*/
#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

using namespace std;

#define DEFAULT_MESSAGE 907

struct VisualData
{
    uint32 Menu;
    uint32 Submenu;
    uint32 Icon;
    uint32 Id;
    string Name;
};

VisualData vData[] =
{
    { 1, 0, GOSSIP_ICON_BATTLE, 3789, "Berserking" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3854, "Spell Power" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3273, "Deathfrost" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3225, "Executioner" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3870, "Blood Draining" },
    { 1, 0, GOSSIP_ICON_BATTLE, 1899, "Unholy Weapon" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2674, "Spellsurge" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2675, "Battlemaster" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2671, "Arcane and Fire Spell Power" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2672, "Shadow and Frost Spell Power" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3365, "Rune of Swordshattering" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2673, "Mongoose" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2343, "Spell Power" },
    { 1, 2, GOSSIP_ICON_TALK, 0, "Next.." },

    { 2, 0, GOSSIP_ICON_BATTLE, 425, "Black Temple Dummy" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3855, "Spell Power III" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1894, "Icy Weapon" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1103, "Agility" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1898, "Lifestealing" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3345, "Earthliving I" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1743, "MHTest02" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3093, "Attack Power vs Undead and Demons" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1900, "Crusader" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3846, "Spell Power II" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1606, "Attack Power" },
    { 2, 0, GOSSIP_ICON_BATTLE, 283, "Windfury I" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1, "Rockbiter III" },
    { 2, 3, GOSSIP_ICON_TALK, 0, "Next.." },
    { 2, 1, GOSSIP_ICON_TALK, 0, "..Back" },

    { 3, 0, GOSSIP_ICON_BATTLE, 3265, "Blessed Weapon Coating" },
    { 3, 0, GOSSIP_ICON_BATTLE, 2, "Frostbrand I" },
    { 3, 0, GOSSIP_ICON_BATTLE, 3, "Flametongue III" },
    { 3, 0, GOSSIP_ICON_BATTLE, 3266, "Righteous Weapon Coating" },
    { 3, 0, GOSSIP_ICON_BATTLE, 1903, "Spirit" },
    { 3, 0, GOSSIP_ICON_BATTLE, 13, "Sharpened" },
    { 3, 0, GOSSIP_ICON_BATTLE, 26, "Frost Oil" },
    { 3, 0, GOSSIP_ICON_BATTLE, 7, "Deadly Poison" },
    { 3, 0, GOSSIP_ICON_BATTLE, 803, "Fiery Weapon" },
    { 3, 0, GOSSIP_ICON_BATTLE, 1896, "Weapon Damage" },
    { 3, 0, GOSSIP_ICON_BATTLE, 2666, "Intellect" },
    { 3, 0, GOSSIP_ICON_BATTLE, 25, "Shadow Oil" },
    { 3, 2, GOSSIP_ICON_TALK, 0, "..Back" },
};



class npc_visualweapon : public CreatureScript
{
public:
    npc_visualweapon() : CreatureScript("npc_visualweapon") { }

    bool MainHand;

    void SetVisual(Player* player, uint32 visual)
    {
        uint8 slot = MainHand ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_OFFHAND;

        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

        if (!item)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("No equipped item available to change the Visual.");
            return;
        }

        const ItemTemplate* itemTemplate = item->GetTemplate();

        if (itemTemplate->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD ||
            itemTemplate->SubClass == ITEM_SUBCLASS_ARMOR_BUCKLER ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_SPEAR ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_obsolete ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC2 ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_MISC ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_THROWN ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_WAND ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_obsolete)
            return;

        player->SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (item->GetSlot() * 2), 0, visual);
        CharacterDatabase.PExecute("REPLACE into custom_item_enchant_visuals VALUES ('%u', '%u', '%s')", item->GetGUIDLow(), visual, player->GetName().c_str());
    }

    void GetMenu(Player* player, Creature* creature, uint32 menuId)
    {
        for (uint8 i = 0; i < (sizeof(vData) / sizeof(*vData)); i++)
        {
            if (vData[i].Menu == menuId)
                player->ADD_GOSSIP_ITEM(vData[i].Icon, vData[i].Name, GOSSIP_SENDER_MAIN, i);
        }

        player->SEND_GOSSIP_MENU(DEFAULT_MESSAGE, creature->GetGUID());
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface/PaperDoll/UI-PaperDoll-Slot-MainHand:40:40:-18|tMain-Hand", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface/PaperDoll/UI-PaperDoll-Slot-SecondaryHand:40:40:-18|tOff-Hand", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:40:40:-18|tNevermind", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        player->SEND_GOSSIP_MENU(DEFAULT_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 1:
            MainHand = true;
            GetMenu(player, creature, 1);
            return false;

        case GOSSIP_ACTION_INFO_DEF + 2:
            MainHand = false;
            GetMenu(player, creature, 1);
            return false;

        case GOSSIP_ACTION_INFO_DEF + 3:
            player->CLOSE_GOSSIP_MENU();
            return false;
        }

        uint32 menuData = vData[action].Submenu;

        if (menuData == 0)
        {
            SetVisual(player, vData[action].Id);
            menuData = vData[action].Menu;
        }

        GetMenu(player, creature, menuData);
        return true;
    }
};

class player_visualweapon : public PlayerScript
{
public:
    player_visualweapon() : PlayerScript("player_visualweapons") {}

    void GetVisual(Player* player)
    {
        if (!player)
            return;

        Item* pItem;

        // We need to query the DB to get item
        QueryResult result = CharacterDatabase.PQuery("SELECT iguid, display FROM custom_item_enchant_visuals WHERE iguid IN(SELECT guid FROM item_instance WHERE owner_guid = %u)", player->GetGUIDLow());

        if (!result)
            return;

        // Lets loop to check item by pos
        for (int i = EQUIPMENT_SLOT_MAINHAND; i < EQUIPMENT_SLOT_OFFHAND; ++i)
        {
            pItem = player->GetItemByPos(255, i);
        }

        // Now we have query the DB we need to get the fields.
        do
        {
            Field* fields = result->Fetch();
            uint32 iguid = fields[0].GetUInt32();
            uint32 visual = fields[1].GetUInt32();

            // Lets make sure our item guid matches the guid in the DB
            if (pItem->GetGUIDLow() == iguid)
            {
                player->SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (pItem->GetSlot() * 2), 0, visual);
            }

        } while (result->NextRow());
    }

    // if Player has item in bag and re-equip it lets check for enchant
    void OnEquip(Player* player, Item* /*item*/, uint8 /*bag*/, uint8 /*slot*/, bool /*update*/) override
    {
        GetVisual(player);
    }

    void OnLogin(Player* p) override
    {
        GetVisual(p);
    }
};

void AddSC_Npc_VisualWeaponScripts()
{
    new npc_visualweapon;
    new player_visualweapon;
}
