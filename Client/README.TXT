This client was writen by Diuuude & Snoopy81.
Based on Cleroth work.
                   ~~~

V3.51 compatibility by Cleroth	
V3.51 dialogs by Diuuude
Effects, mobs, Apocalypse, Heldenian
& finalizing by Snoopy81
V3.82 Angels / Crafting by Snoopy81

___________________________________________________

This source is suposed to offer a client very 
close to v3.82; but the aim is not to have an 
exact copy. Some images, windows, texts may be 
slightly different!
The code is not always exactelly based on v351,
we sometimes used different ways to match a similar
result.
We also fixed some v382 bugs or issues.

The code has been cleared of most corean comments,
and some comments have been added, mostlly English
but some French comments survived!

Somes features has been added or fixed. You may use the
compilation options (see GlobalDef.h). 
They are commented!
Some very important data has been moved to this file as
well, such as server name, port, IP, version...

You may used the client with LOGIN.CFG to use a
dynamic IP.

Note that even if I already coded some new features for
this client, the aim of this sources is only to be as
close as possible to v3.51 official client. So, most 
of those extra features have been removed.
___________________________________________________

The Angels & crafting msg have been reversed from (partially)
uncrypted 3.60 & 3.81 clients. AS I have little experience of
those clients, I did not built an exact copy. As usual I improved
some things.
As there is no siementec client+server available above v3.51,
I didn't have enought details to aim a perfect copy. 

Crafting is strangelly coded in client 382, as if it was intended to 
be coded as Manuf., and later made simpler as Alchemy. I 
choose to code it this way. The Magin saphirre definition has
been changed, for simpler coding server side; and 
independence from item number or name.
Crafting items are in the potion.cfg, so you may adjust them.

Angels inial codelets wer found on this forum, but I realised
that the stats changes drove to a big mess!
Then I designed another system, based on stats adjustement,
and realised when reversing later clients, that I was right.
The Siementec client receives stats adjustements from server,
but my client is able to compute that by himself. So I didn't
code server sending stats adjustments.
The Siementec's Angels status is already in use in my current
server, and I initially used other ones for my 1st Angel codes.
This client and server codes included match together, but not
with Siementec's ones.


___________________________________________________

What is missing to match v3.82 client:
- Druncncity effect.
As I have no experience a the Siementec v3.82 client, many
things may look different, or work in a slightlly different way.

___________________________________________________

What has been changed/improved from v3.82 client:
- Abaddon's death/corpse.
- Fire Wyvern's death/corpse.
- Use of LOGIN.CFG
- Optional Account creator.
- Optional Server choice.
- Optional numeric format.
- Msg "Adminuser level low".
- Added router compatibility mode (see LOGIN.CFG).
- No more client-side blocking for some TPs.
- Some more explanations for Gail's window.
- No need for cximage.dll (code is included, that
  explains bigger exe).
- Removed some msg when dead (too much msg.
  scrolls out the 1st one!).
- Various typo fixed.
- Added file version info.
- Status for showing Angels is not the same. It allows more
  grades in "Stars" effect. Anyways it's compatible with released
  server-side code.

___________________________________________________

v382 known bugs fixed:
- MasterElf arrows.
- Ek anouncer (Name/guild was exchanged).
- Abaddon's death when facing NW.
- Longer top msg when blocked in jail.
- Faster "Level Up" showing.
- Fixed ObserMode.
- Energy Ball fixed.
- Ugly Wyvern fixed.

___________________________________________________


