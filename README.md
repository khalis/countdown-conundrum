# Countdown conundrum

This program is meant for solving puzzles from Countdown game, Channel 4 (8 out of 10 cats version).

This solves both letters and numbers puzzles, however there are some parts that can be improved.

Letters puzzle solver relies on a dictionary from [github.com/dwyl/english-words](https://github.com/dwyl/english-words), which is not the same as the official dictionary used on the programme. Proper solution to this discrepancy is to use list of words from  Oxford dictionary, but as far as I'm aware such a list is not available for download (it of course also needs to be licensed). When in doubt I would suggest consulting OED's official site.

## Usage

To solve letter puzzle just type the letters in lower case with no spaces and hit return. It will return all words that it can find, top 4 letter lengths -- usually there are too many solutions of lengths 4, 3, and they are not that interesting anyway.

Example:
```
Enter puzzle: ewabejsru
Found 306 solutions

7
abjures bewares reabuse

6
bajree abusee jabers abjure abuser beware brujas rebase bursae busera reseau erebus webers subjee subwar urease weaser

5
barse brees besew besra baser baure rajes rubes ewers bears resue aures beaus awber jebus brews beers braes bruja reuse buras bursa sabre burse rebus resew abrus jeers easer resaw jubas erase braws saree sawer serab serau seraw sewar absee suber bares subra abuse sware swear sweer swure sewer ureas saber ursae wares warse wears jubes weber urase

4
jear besa rube brae bras bree brew reub juba bura braw bure burs erse esau jura jeer rebs ajee sabe eure ears ease arbs jees ares arew rees jere eras jews jesu arse jube sare ewer saur ruse sear seba seer sera ewes aube rues serb sere raws awee suba awes subj rubs rusa rase suer sura sure suwe swab bare bars baru jabs base urbs urea jars bear ursa beau usar usee user wabe wabs waer waes warb ware jure wars beer wase waur wear bees beja jaws bere webs weer wees were wese
```

To solve numbers puzzle type all numbers with spaces in between, with the goal as the last number. Solver will print first 10 solutions, if found.
```
Enter puzzle: 25 100 1 7 9 5 729
Found 13 solutions, here are 10:
25+(9-1)*(100-(7+5)) = 729
9*(100+1)-(5+25*7) = 729
9+(7-1)*(25+100-5) = 729
9*(100-(25-(1+5))) = 729
9*(100-(1+25-7)) = 729
1+7*(100-(25/5)+9) = 729
1+7*(100-5+9) = 729
5*(7*25-9)-(1+100) = 729
9*(5+7*25-100+1) = 729
25+5+7*100-1 = 729
```
