#include "skindesignerosdbase.h"

/**********************************************************************
* cSkindesignerOsdItem
**********************************************************************/
skindesignerservice::cSkindesignerOsdItem::cSkindesignerOsdItem(eOSState State) : cOsdItem(State) {
    sdDisplayMenu = NULL;
}

skindesignerservice::cSkindesignerOsdItem::cSkindesignerOsdItem(const char *Text, eOSState State, bool Selectable) : cOsdItem(Text, State, Selectable) {
    sdDisplayMenu = NULL;
}  

skindesignerservice::cSkindesignerOsdItem::~cSkindesignerOsdItem() {
}

void skindesignerservice::cSkindesignerOsdItem::SetMenuItem(cSkinDisplayMenu *DisplayMenu, int Index, bool Current, bool Selectable) {
    if (sdDisplayMenu) {
        if (!sdDisplayMenu->SetItemPlugin(&stringTokens, &intTokens, &loopTokens, Index, Current, Selectable)) {
            DisplayMenu->SetItem(Text(), Index, Current, Selectable);
        }
    } else {
        DisplayMenu->SetItem(Text(), Index, Current, Selectable);
    }
}

void skindesignerservice::cSkindesignerOsdItem::AddStringToken(string key, string value) {
    stringTokens.insert(pair<string,string>(key, value));
}

void skindesignerservice::cSkindesignerOsdItem::AddIntToken(string key, int value) {
    intTokens.insert(pair<string,int>(key, value));
}

void skindesignerservice::cSkindesignerOsdItem::AddLoopToken(string loopName, map<string, string> &tokens) {
    map<string, vector<map<string, string> > >::iterator hitLoop = loopTokens.find(loopName);
    if (hitLoop == loopTokens.end()) {
        vector<map<string, string> > tokenVector;
        tokenVector.push_back(tokens);
        loopTokens.insert(pair<string, vector<map<string, string> > >(loopName, tokenVector));
    } else {
        vector<map<string, string> > *tokenVector = &hitLoop->second;
        tokenVector->push_back(tokens);
    }
}


/**********************************************************************
* cSkindesignerOsdMenu
**********************************************************************/
skindesignerservice::cSkindesignerOsdMenu::cSkindesignerOsdMenu(const char *Title, int c0, int c1, int c2, int c3, int c4) : cOsdMenu(Title, c0, c1, c2, c3, c4) {
    init = true;
    displayText = false;
    sdDisplayMenu = NULL;
    pluginName = "";
    SetMenuCategory(mcPlugin);
    SetSkinDesignerDisplayMenu();
}

skindesignerservice::cSkindesignerOsdMenu::~cSkindesignerOsdMenu() {
}

void skindesignerservice::cSkindesignerOsdMenu::SetPluginMenu(int menu, eMenuType type) {
    if (type == mtList)
        displayText = false;
    else if (type == mtText)
        displayText = true;

    if (sdDisplayMenu) {
        sdDisplayMenu->SetPluginMenu(pluginName, menu, type, init);
    }
    init = false;
}

bool skindesignerservice::cSkindesignerOsdMenu::SetSkinDesignerDisplayMenu(void) {
    sdDisplayMenu = ISkindesignerService::CallGetDisplayMenu();
    return (sdDisplayMenu != NULL);
}

void skindesignerservice::cSkindesignerOsdMenu::ClearTokens(void) {
    text = "";
    stringTokens.clear();
    intTokens.clear();
    loopTokens.clear();
}

void skindesignerservice::cSkindesignerOsdMenu::AddStringToken(string key, string value) {
    stringTokens.insert(pair<string,string>(key, value));
}

void skindesignerservice::cSkindesignerOsdMenu::AddIntToken(string key, int value) {
    intTokens.insert(pair<string,int>(key, value));
}

void skindesignerservice::cSkindesignerOsdMenu::AddLoopToken(string loopName, map<string, string> &tokens) {
    map<string, vector<map<string, string> > >::iterator hitLoop = loopTokens.find(loopName);
    if (hitLoop == loopTokens.end()) {
        vector<map<string, string> > tokenVector;
        tokenVector.push_back(tokens);
        loopTokens.insert(pair<string, vector<map<string, string> > >(loopName, tokenVector));
    } else {
        vector<map<string, string> > *tokenVector = &hitLoop->second;
        tokenVector->push_back(tokens);
    }
}

void skindesignerservice::cSkindesignerOsdMenu::TextKeyLeft(void) {
    if (!displayText)
        return;
    DisplayMenu()->Scroll(true, true);
}

void skindesignerservice::cSkindesignerOsdMenu::TextKeyRight(void) {
    if (!displayText)
        return;
    DisplayMenu()->Scroll(false, true);
}

void skindesignerservice::cSkindesignerOsdMenu::TextKeyUp(void) {
    if (!displayText)
        return;
    DisplayMenu()->Scroll(true, false);
}

void skindesignerservice::cSkindesignerOsdMenu::TextKeyDown(void) {
    if (!displayText)
        return;
    DisplayMenu()->Scroll(false, false);
}

void skindesignerservice::cSkindesignerOsdMenu::Display(void) {
    if (displayText) {
        if (sdDisplayMenu) {
            if (sdDisplayMenu->SetPluginText(&stringTokens, &intTokens, &loopTokens)) {
                esyslog("skindesclient: template found");
                sdDisplayMenu->Flush();
            } else {
                esyslog("skindesclient: no template found, drawing default");
                DisplayMenu()->Clear();
                DisplayMenu()->SetText(text.c_str(), false);
                DisplayMenu()->Flush();
            }
    } else {
            DisplayMenu()->Clear();
            DisplayMenu()->SetText(text.c_str(), false);
            DisplayMenu()->Flush();
        }
        return;
    }
    if (sdDisplayMenu) {
        for (cOsdItem *item = First(); item; item = Next(item)) {
            cSkindesignerOsdItem *sdItem = dynamic_cast<cSkindesignerOsdItem*>(item);
            if (sdItem)
                sdItem->SetDisplayMenu(sdDisplayMenu);
        }
    }
    cOsdMenu::Display();
}
