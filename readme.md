# Game of Presently Unknown Name for Nintendo 64
Yep that is correct, we are gonna be making a real sweet nintendo 64 game that
combines the best of the best with other shit that I like. The primary purpose
of this document is to be a record of the idea and vibe of the game so that I
can keep track, and reignite my excitement when I forget what the point of the
game is.

## General overview of gameplay
The world will feel a bit like mario 64 or banjo kazooie in that it has what you
would definitely call levels, they have real visual distinctness and identity.
However, rather than being connected by a single hub world, these levels are
connected to one another through normal means like going through a doorway etc,
and sometimes in order to get access to a certain level you will need to
unlock special moves / items. (not entirely sure what that will be like yet).

That will just be metroidvania style unlocking items and exploring and pressing
switches etc. The game will feature one boss in every level, and in order to
fight this boss, you have to expend a certain number of "power stars" that you
have collected. Would also be cool if you can expend power stars on other
stuff as well, just as long as the total amount of power stars you can spend
is equal to the total number in the game. Beating bosses will not be relevant
to progression in the game except for reaching the final area and boss, so
blowing your power stars early is not an issue other than the fact you will
need to play harder. Maybe the people who sell stuff for stars will only sell
one each and be not close together in the game so people can't unthinkingly blow
all their stars until they've likely played long enough to know better.

I think it would be nice to make it that beating bosses increases your max hp a
bit, like in zelda. This way collecting stars does still have a certain amount
of relationship to your ability to progress since progressing with no extra
health will be quite hard. This also gives you a nice reason to go back and do
bosses later if you missed them, will feel nice to go back to what you are doing
but with more health. Obviously though I think the degree to which health scales
in zelda games is a bit ridiculous and I don't want it to feel like an rpg game
where a late game character vs early game enemies is just like a knife through
butter. If you started with the equivalent of three hearts in zelda, maybe
each boss would just give you half a heart extra rather than a whole one. There
can also be a couple of these half hearts hidden out in the world.

### Level continuity
One thing about mario 64 is that 

## Game World
At least as far as initial planning goes (I can add more later or remove if I am
really lazy), the game will feature 5 levels:
 - nice starter grassy level with a village and castle etc
 - caves level that also connects to levels disconnected above ground. will be
   inhabited by some kind of small camp/village thing.
 - a mountain area with lots of pain in the ass platforming bits. Might feature
   a single hermit if anybody. connects to the caves.
 - a river or swamp area. There will be enough characters already at this point
   so probably just add one weirdo. Would be cool if the only way to access this
   area is by getting into the sewers of the city and then going out of a drain
   pipe. However, perhaps once you've accessed this area, there is a way that
   you can make it accessible from the caves or the plains or something. Would
   be cool to have a star where you need to go into a building that is resting
   on a giant turtle's back or something like that. Or maybe a boat that is
   sailing down the river.

There will also be a city area where you can consult a guy who can give you info
about stuff you have encountered, and there will be maybe a shop or two and some
people giving you hints about locations of stuff, and maybe you can get some
bonus "power stars" from people here who give you quest type things. This city
area is also where you access the end of the game area.

The story as I see it now is one I have toyed with before which had a name like
"The Earthly Terrors" (although I think there might be five or six now).

## Controls
stick to move
a to jump
b to use your equipped item
left and right c to rotate camera (will play with making it rotate smoothly as
opposed to mario 64 where it jumps in increments).
up and down c buttons and the right trigger will be equippable sort of similarly
to zelda, and you will be able to use items from them.

Regarding equippable items, where this differs from zelda is that you will
(almost) always have some kind of item equipped in your hand, and then you will
generally press b to activate that item (some items might be passive or affect
a different move in which case b might do nothing or have a secondary effect
like a weak attack). Things like a sword for attacking or a function for looking
around in first person will be implemented as items rather than having dedicated
buttons.

In a way, we don't really need to think of the use of the item as being an
exclusively B button thing, essentially having an item equipped changings your
whole moveset to some degree (well, sometimes just B), and there are plenty of
items that would benefit from modifying a number of actions. For example,
pressing Z in the air with a sword might make you do a downward sword attack
that moves you very quickly downwards, while doing the same with the bow
equipped might make you shoot directly downwards and hop upwards very slightly.

So the role of the B button is not so much "use your equipped item" as
"primary action in the given context" which will often be doing something with
your equipped item but it could also be talking to people etc.

### Inventory and User Experience
Also good to note that having to go to the menu to change what items you have
equipped is kind of annoying, so at the very least the menu should be as
responsive and quick to open as possible. No circle menus though I hate that
shit, 90% of the time I select the wrong thing by mistake and that's on modern
consoles, I can only imagine it would be worse with an n64 control stick. Would
be worth thinking about if there is a low key way of doing the same thing
though. Maybe even just when you open the menu it is open to the item screen by
default and the selection is on the item in the middle so you have minimum
average distance to all items. If we wanted to be even more fancy we could sort
the items so that the most recently used are closer to the starting point of
the cursor.

I have thought about this some more and to be honest, the issue really isn't so
much that using the inventory is slow, it's more that it is just a kind of
uncomfortable feeling when you have to set up your items in such a way that you
know you are going to have to change them again soon, so really the important
thing is not so much speeding up the menu (still don't waste time on
superfluous animations etc), but rather avoiding making the user change their
setup all the time. One way of doing this would be to make it that if there are
items that just do a specific thing and you only do it occasionally, maybe you
could just use it directly from the menu rather than having to bind it to a
button first.

### Basic Moveset
Yeah here's the moves assuming you've got like a sword or something
equipped:
 - long jump like mario 64. good for platforming and feels good because of the
   speed. an essential move.
 - side jump like mario 64. another essential part of moving fast I think, tying
   high jumps to movement makes it feel good, and it's good for turning around.
 - dive like mario 64. This move gives you more mobility in the air and lets you
   speed up and feels good.
 - wall bouncing. There is an argument to be made that this should be an
   unlockable feature rather than something you start with since it aids
   progression in a way that would make a very nice upgrade check, but I sort of
   dislike it when metroidvania games make your traversal bad at the start and
   then by the end the game is basically broken by how many traversal moves you
   have. Eh maybe I'm overthinking it, but I'd prefer the upgrade checks to be
   more about having special items like bow/hookshot type things. I might add a
   double jump item though.
 - normal attack. Yeah you swing your sword in front of you and can hit
   stuff. this doesn't really change while moving, maybe it slows you down a
   bit.

There are some moves from mario 64 however that are not worth emulating imo. The
backflip jump is redundant when you have the side jump that goes about as high
and is faster and more fluid and gives you more control. (if I feel like
implementing a super high jump at some point though I guess it can be an item).
Also I don't think the triple or double jumps are particularly important. I
think having seperate jump animations and height for whether you are stationary
or moving could be a nice feature but triple jumps are not important to me.

## Combat
This is the big question everybody is asking. How will combat work? My main
inspiration gameplay wise is mario 64. But mario 64 barely features anything you
could call combat and the boss fights in that game are not satisfying. The
reason I say that is every major fight in mario 64 can be completed in less than
30 seconds, and rather than being a case of risking your life to speed run
them, you are actually much less likely to die the more you rush through fights.

Obviously taking too long never means you are doing well exactly, but I think
that there is a sweet spot between going too fast and going too slow at which
you are at your safest, and I think in mario 64 that sweet spot is way too
close to the fast side than the slow side and ultimately there are very few
actions you actually need to successfully complete in order to defeat a boss,
and if a boss takes more than 15 seconds to beat it's not because they are
strong but because of imperfections in your playing.

Short boss battles are unsatisfying because you end up spending very little time
with the enemy, which when they are supposed to look cool and scary, you need to
make the player spend some time with them to appreciate their coolness and feel
like they are fighting a great battle etc etc etc.

So firstly, this game will feature a sword weapon, and probably ranged weapons
as well. How do we imagine that this is going to work? A ranged weapon that you
actively have to aim in first person every time you want to use it is not going
to be very much fun to use in combat, but the control scheme I have envisioned
doesn't leave much room for things like z targeting. Lets say perhaps normally
when you fire the bow it shoots in front of you, but if you press b while also
crouching then it goes into a first person aiming mode. If aiming without z
really sucks we can consider just making it auto aim at the nearest enemy or
something when z is not used. Ok that honestly sounds like an acceptable combat
system. It will be fast and fluid and feel like it's about more than just timed
button presses (how ocarina of time feels), but it will also be able to have
longer more complex confrontations.

On the topic of the bow, I think it should have infinite ammo but you should
need to reload it after one or two shots, which should require you to be
stationary, or at least not attacking. This way it can feel powerful but not be
OP. Maybe it will be a crossbow. Also, it would be cool if the downward air
attack fires all your arrows in one go or something like that.