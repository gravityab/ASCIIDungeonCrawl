# ASCII Dungeon Crawl
Dungeon Crawl is a simple procedural commandline ASCII dungeon delving game that gets exponentially harder the further your party traverses in the dungeon. The goal is to get as far as you can before getting defeated.

![image](https://github.com/user-attachments/assets/fd0416d4-98e1-480c-acc5-10d303822b72)

### Buttons
```
UP, LEFT, RIGHT, DOWN - Movement
ENTER - Select
BACKSPACE - Back
RSHIFT - Toggle Statistics
```

## Doors
Every floor the player is given a decision as to which door they wish to take. Doors come in various levels of rarity and look different based on the potential outcomes. Typically doors with higher quality have more or even higher level monsters, but give better rewards.
![image](https://github.com/user-attachments/assets/fc1db7e2-efcb-4ee4-8e59-3bd4cc108902)

## Shop
Throughout the dungeon there are shops. Here you can make decisions as to what weapons, items, or armor you can equip your party with to take into the dungeon.
![image](https://github.com/user-attachments/assets/6ef414cc-7b42-40e6-81f5-f6456ed9aa59)

## Weapons
|Gloves|Dagger|Sword|Great Sword|
|---|---|---|---|
|Damage: {1,2}d4 + {1,3}<br>Speed: 0<br>Cost: 2d4 + 4|Damage: {1,3}d4 + {1, 10}<br>Speed: {1,4}<br>Cost:2d4 + 4|Damage: {1,4}d6 + {1,6}<br>Speed: {3,6}<br>Cost: 2d6 + 6|Damage: {1,4}d10 + {1,10}<br>Speed: {8,15}<br>Cost: 2d10 + 10|
|![Knuckles](https://github.com/user-attachments/assets/1094d21a-a65f-439e-9c89-f0f549d46a2d)|![Dagger](https://github.com/user-attachments/assets/55750dee-7a86-4167-983e-77cb1372841a)|![Sword](https://github.com/user-attachments/assets/969d0b16-02d6-445a-870b-1fe6436ebd3d)|![Great Sword](https://github.com/user-attachments/assets/daa5d220-485d-492f-8e1c-4d573f3e28af)

|Wand|Staff|
|---|---|
|Damage: {2,4}d4 + {1,5}<br>Speed: {2,5}<br>MP: {4,5}<br>Cost: 2d4 + 4|Damage: {3,5}d8 + {1,10}<br>Speed: {8,10}<br>MP: {8,15}<br>Cost: 2d8 + 8<br>Properties: Multi Target|
|![Wand](https://github.com/user-attachments/assets/c150a96b-6068-4863-8049-1bf59bf239cc)|![Staff](https://github.com/user-attachments/assets/e684ad1b-fe32-4f02-b53d-baed0c892461)|

## Armor
|Robe|Leather|Plate|
|---|---|---|
|AC: {1,2}<br>Cost: 4d4 + 4<br>Property: Magic Boost, Magic Damage|AC: {2,3}<br>Cost: 4d8 + 4<br>Property: XP Boost|AC: {3,4}<br>Cost: 4d10 + 4<br>Property: HP Boost|
|![Robe](https://github.com/user-attachments/assets/1a62fc60-e865-42f7-9abf-4c960b1e7713)|![Leather](https://github.com/user-attachments/assets/30a1872a-76d9-460f-92c5-2231e936f0da)|![Plate](https://github.com/user-attachments/assets/07e325ea-8366-45af-8e16-ed75eec6105a)|

## Potions
|Lesser Health Potion|Health Potion|Greater Health Potion|
|---|---|---|
|Heal: 2d8 + {1,4}<br>Cost: 1d8 + 8|Heal: 4d8 + {4,8}<br>Cost: 2d8 + 8|Heal: 8d8 + {8,12}<br>Cost: 3d8 + 8|
|![Lesser Healing Potion](https://github.com/user-attachments/assets/8a6137ea-bd12-4034-a334-baf2688f4ad6)|![Healing Potion](https://github.com/user-attachments/assets/21267813-9704-4a85-ac30-ee1ef4bb56bd)|![Greater Healing Potion](https://github.com/user-attachments/assets/30f4aaa5-176c-4606-86e3-0947bebef1b8)|

|Lesser Magic Potion|Magic Potion|Greater Magic Potion|
|---|---|---|
|Restore: 2d8 + {1,4}<br>Cost: 1d8 + 8|Restore: 4d8 + {4,8}<br>Cost: 2d8 + 8|Restore: 8d8 + {8,12}<br>Cost: 3d8 + 8|
|![Lesser Magic Potion](https://github.com/user-attachments/assets/10552c47-a10d-4b2a-8211-1f68405862d7)|![Game Potion](https://github.com/user-attachments/assets/ce557771-ce12-442d-a64e-eae412f419f7)|![Greater Magic Potion](https://github.com/user-attachments/assets/df5d1512-230c-4bcd-92aa-4db4a6d99f47)|

## Other
|New Hero|Level Up|Level Up 5|
|---|---|---|
|Cost: 10d10 + 10<br>Property: New Hero|Cost: 10d10 + 10<br>Property: Level + 1|Cost: 45d10 + 10<br>Property: Level + 5|
|![New Hero](https://github.com/user-attachments/assets/5f9b4b1d-3914-446a-9ede-a66db847536e)|![Level Up](https://github.com/user-attachments/assets/8f10774f-f84d-4b75-b2ba-17566c459860)|![Level Up 5](https://github.com/user-attachments/assets/7cb74cad-38bb-4e20-8b8e-183c48041878)

## Combat
Combat is turn based and purchased weapons can be used to battle.
![image](https://github.com/user-attachments/assets/dadcf582-2eff-41f0-a6b1-5435cdb9f934)
### Elemental Counters
```
Cold ---> Lightning         Normal         Psychic       Holy        Healing
  ^           |            ^      \           ^           ^
  |           V           /        v          v           v
Fire <---- Water       Poison <-- Steel      Dark      Necrotic
```

## Traps
Throughout the dungeon there are trap rooms. These rooms require less faster more mobile equipment to not run the risk of falling prey to traps.
![image](https://github.com/user-attachments/assets/1594d89c-b4f8-4c99-bcea-b32eff325559)

## Fountain
Scattered throughout the dungeon there are Fountain rooms with fairies that will heal and resurrect your party to full if you're lucky enough.
![image](https://github.com/user-attachments/assets/e9d30060-8e62-4efd-9d2f-d2a415c5df37)

## Bosses
Bosses are lurking at the lower levels should the adventures delve deep enough
![image](https://github.com/user-attachments/assets/6ebd1795-d279-4ded-a5e1-7831969efe86)

# Credits
This game is dedicated to both my 7 year old sons. I am using this simple game to teach my kids programming and game design.

> NOTE: Once a week I sit down with my boys and go over how to program. I usually introduce them to a new concept or syntax then make a new game from scratch using whatever subject we are trying to learn. In the past we've made simple games like Simon Says, Calculators, Horse Racing, etc; each with their own challenges for the boys to learn. This is the first time I wanted to push the code to a public repo for everyone to enjoy my sons creation.
