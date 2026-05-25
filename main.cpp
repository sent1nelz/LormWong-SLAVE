#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include <optional>

// Set ขนาด UI
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

// สี UI สำหรับใช้ในส่วนต่างๆ ของกระดานและปุ่ม
const sf::Color GREEN_FELT(30, 110, 50, 255);
const sf::Color GRAY_BG(170, 170, 170, 255);
const sf::Color GRAY_TEXT(100, 100, 100, 255);
const sf::Color RED_SUIT(220, 20, 60, 255);
const sf::Color BLACK_SUIT(10, 10, 10, 255);
const sf::Color GOLD_H(255, 215, 0, 255);
const sf::Color DARK_BG(45, 45, 45, 255);
const sf::Color MSG_BG(20, 20, 20, 255);

// Ranking ไพ่ (เรียงจากน้อยไปมาก 3 เล็กสุด, 2 ใหญ่สุด)
std::vector<std::string> RANKS = { "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2" };
// ลำดับดอกไพ่ (เรียงจากน้อยไปมาก ดอกจิก, ข้าวหลามตัด, โพแดง, โพดำ)
std::vector<char> SUITS = { 'C', 'D', 'H', 'S' };

// วาดรูปดอกบนไพ่ โดยใช้ Vector Graphics ของ SFML
void DrawSuit(sf::RenderWindow& window, char suit, float cx, float cy, float size, sf::Color color) {
    if (suit == 'D') { // Diamond (ข้าวหลามตัด)
        sf::ConvexShape diamond(4);
        diamond.setPoint(0, sf::Vector2f({cx, cy - size}));
        diamond.setPoint(1, sf::Vector2f({cx + size * 0.7f, cy}));
        diamond.setPoint(2, sf::Vector2f({cx, cy + size}));
        diamond.setPoint(3, sf::Vector2f({cx - size * 0.7f, cy}));
        diamond.setFillColor(color);
        window.draw(diamond);
    }
    else if (suit == 'H') { // Heart (โพแดง)
        float r = size * 0.5f;
        sf::CircleShape leftLob(r);
        leftLob.setFillColor(color);
        leftLob.setOrigin({r, r});
        leftLob.setPosition({cx - r, cy - r * 0.5f});

        sf::CircleShape rightLob(r);
        rightLob.setFillColor(color);
        rightLob.setOrigin({r, r});
        rightLob.setPosition({cx + r, cy - r * 0.5f});

        sf::ConvexShape bottomTri(3);
        bottomTri.setPoint(0, sf::Vector2f({cx - size, cy - r * 0.5f}));
        bottomTri.setPoint(1, sf::Vector2f({cx + size, cy - r * 0.5f}));
        bottomTri.setPoint(2, sf::Vector2f({cx, cy + size}));
        bottomTri.setFillColor(color);

        window.draw(leftLob);
        window.draw(rightLob);
        window.draw(bottomTri);
    }
    else if (suit == 'S') { // Spade (โพดำ)
        float r = size * 0.5f;
        sf::CircleShape leftLob(r);
        leftLob.setFillColor(color);
        leftLob.setOrigin({r, r});
        leftLob.setPosition({cx - r, cy + r * 0.5f});

        sf::CircleShape rightLob(r);
        rightLob.setFillColor(color);
        rightLob.setOrigin({r, r});
        rightLob.setPosition({cx + r, cy + r * 0.5f});

        sf::ConvexShape topTri(3);
        topTri.setPoint(0, sf::Vector2f({cx - size, cy + r * 0.5f}));
        topTri.setPoint(1, sf::Vector2f({cx + size, cy + r * 0.5f}));
        topTri.setPoint(2, sf::Vector2f({cx, cy - size}));
        topTri.setFillColor(color);

        sf::RectangleShape stem(sf::Vector2f({r * 0.8f, size}));
        stem.setFillColor(color);
        stem.setOrigin({r * 0.4f, 0.f});
        stem.setPosition({cx, cy + r * 0.5f});

        window.draw(leftLob);
        window.draw(rightLob);
        window.draw(topTri);
        window.draw(stem);
    }
    else if (suit == 'C') { // Club (ดอกจิก)
        float r = size * 0.45f;
        sf::CircleShape topLob(r);
        topLob.setFillColor(color);
        topLob.setOrigin({r, r});
        topLob.setPosition({cx, cy - size * 0.4f});

        sf::CircleShape leftLob(r);
        leftLob.setFillColor(color);
        leftLob.setOrigin({r, r});
        leftLob.setPosition({cx - r * 1.1f, cy + size * 0.2f});

        sf::CircleShape rightLob(r);
        rightLob.setFillColor(color);
        rightLob.setOrigin({r, r});
        rightLob.setPosition({cx + r * 1.1f, cy + size * 0.2f});

        sf::RectangleShape stem(sf::Vector2f({r * 0.8f, size * 0.8f}));
        stem.setFillColor(color);
        stem.setOrigin({r * 0.4f, 0.f});
        stem.setPosition({cx, cy + size * 0.2f});

        window.draw(topLob);
        window.draw(leftLob);
        window.draw(rightLob);
        window.draw(stem);
    }
}

// วาดสี่เหลี่ยมขอบมน (ไพ่และปุ่ม)
void DrawRectangleShapeRounded(sf::RenderWindow& window, sf::FloatRect rect, float radius, sf::Color fillColor, sf::Color outlineColor, float outlineThickness) {
    sf::RectangleShape shape(sf::Vector2f({rect.size.x - 2 * radius, rect.size.y - 2 * radius}));
    shape.setFillColor(fillColor);
    shape.setPosition({rect.position.x + radius, rect.position.y + radius});

    sf::RectangleShape hShape(sf::Vector2f({rect.size.x, rect.size.y - 2 * radius}));
    hShape.setFillColor(fillColor);
    hShape.setPosition({rect.position.x, rect.position.y + radius});
    
    sf::RectangleShape vShape(sf::Vector2f({rect.size.x - 2 * radius, rect.size.y}));
    vShape.setFillColor(fillColor);
    vShape.setPosition({rect.position.x + radius, rect.position.y});

    sf::CircleShape topLeft(radius); topLeft.setFillColor(fillColor); topLeft.setPosition({rect.position.x, rect.position.y});
    sf::CircleShape topRight(radius); topRight.setFillColor(fillColor);
    topRight.setPosition({rect.position.x + rect.size.x - 2 * radius, rect.position.y});
    sf::CircleShape bottomLeft(radius); bottomLeft.setFillColor(fillColor); bottomLeft.setPosition({rect.position.x, rect.position.y + rect.size.y - 2 * radius});
    sf::CircleShape bottomRight(radius); bottomRight.setFillColor(fillColor); bottomRight.setPosition({rect.position.x + rect.size.x - 2 * radius, rect.position.y + rect.size.y - 2 * radius});

    window.draw(hShape);
    window.draw(vShape);
    window.draw(shape);
    window.draw(topLeft);
    window.draw(topRight);
    window.draw(bottomLeft);
    window.draw(bottomRight);

    if (outlineThickness > 0) {
        sf::RectangleShape outline(sf::Vector2f({rect.size.x, rect.size.y}));
        outline.setPosition({rect.position.x, rect.position.y});
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(outlineColor);
        outline.setOutlineThickness(outlineThickness);
        window.draw(outline);
    }
}

//  Class Card
class Card {
private:
    std::string rank; // ตัวเลข/ตัวอักษรหน้าไพ่
    char suit;        // สัญลักษณ์ดอกไพ่
    int power;        // ค่าพลังรวมของไพ่
    int rankVal;      // ค่าของหน้าไพ่
    
    // ตัวแปรสถานะที่เกี่ยวกับ UI/State
    sf::FloatRect rect; // พื้นที่กรอบสี่เหลี่ยมสำหรับการคลิก
    bool isSelected;    // ไพ่ใบนี้ถูกคลิกเลือกอยู่หรือไม่
    bool isPlayable;    // ไพ่ใบนี้สามารถกดลงได้หรือไม่ในเทิร์นนี้

public:
    Card(std::string r, char s, int p, int rv) : rank(r), suit(s), power(p), rankVal(rv), isSelected(false), isPlayable(true) {}

    // Read-Only Parameter
    std::string getRank() const { return rank; }
    char getSuit() const { return suit; }
    int getPower() const { return power; }
    int getRankVal() const { return rankVal; }
    bool getIsRed() const { return suit == 'H' || suit == 'D'; }
    sf::FloatRect getRect() const { return rect; }
    bool getIsSelected() const { return isSelected; }
    bool getIsPlayable() const { return isPlayable; }

    // Set Parameter
    void setRect(const sf::FloatRect& newRect) { rect = newRect; }
    void setSelected(bool selected) { isSelected = selected; }
    void toggleSelected() { isSelected = !isSelected; }
    void setPlayable(bool playable) { isPlayable = playable; }

    // ฟังก์ชันวาดไพ่ลงบนหน้าจอ
    void Draw(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isHovered = false, bool faceUp = true) {
        rect = sf::FloatRect({x, y}, {95.f, 135.f});
        
        // เด้งไพ่ขึ้นเล็กน้อยเมื่อเมาส์ชี้ เด้งสูงขึ้นเมื่อถูกเลือก
        if (isHovered && !isSelected && isPlayable) rect.position.y -= 15;
        else if (isSelected) rect.position.y -= 30;

        if (!faceUp) {
            DrawRectangleShapeRounded(window, rect, 8.f, DARK_BG, sf::Color::White, 2.0f);
            sf::FloatRect inner({rect.position.x + 5.f, rect.position.y + 5.f}, {rect.size.x - 10.f, rect.size.y - 10.f});
            DrawRectangleShapeRounded(window, inner, 6.f, RED_SUIT, sf::Color::Transparent, 0.f);
            return;
        }

        sf::Color bgColor = isPlayable ? sf::Color::White : GRAY_BG;
        sf::Color outColor = isSelected ? GOLD_H : BLACK_SUIT;
        float outThickness = isSelected ? 4.0f : 2.0f;
        DrawRectangleShapeRounded(window, rect, 8.f, bgColor, outColor, outThickness);

        // วาดตัวเลขและสัญลักษณ์
        sf::Color textColor = isPlayable ? (getIsRed() ? RED_SUIT : BLACK_SUIT) : GRAY_TEXT;
        sf::Text text(font, rank, 24);
        text.setFillColor(textColor);
        text.setStyle(sf::Text::Bold);
        text.setPosition({rect.position.x + 8.f, rect.position.y + 5.f});
        window.draw(text);
        DrawSuit(window, suit, rect.position.x + 20.f, rect.position.y + 48.f, 8.0f, textColor);
        DrawSuit(window, suit, rect.position.x + rect.size.x / 2.f, rect.position.y + rect.size.y / 2.f + 10.f, 20.0f, textColor);
    }
};

//  Board Class
class Board {
protected:
    std::vector<std::vector<Card>> tableCards; // เก็บประวัติไพ่ที่ถูกลงมาบนโต๊ะ
public:
    virtual ~Board() = default;
    const std::vector<std::vector<Card>>& getTableCards() const { return tableCards; }
    bool isTableEmpty() const { return tableCards.empty(); } // ตรวจสอบว่าโต๊ะว่างไหม
    const std::vector<Card>& getLastPlayed() const { return tableCards.back(); } // ดูไพ่ชุดล่าสุดที่ถูกลง
    virtual void ClearBoard() { tableCards.clear(); } // เคลียร์ไพ่ทั้งหมดออกจากโต๊ะ
};

// UI Class
class UIComponent {
protected:
    sf::FloatRect rect;
public:
    bool isEnabled; // สถานะการเปิด/ปิดปุ่ม
    UIComponent(sf::FloatRect r) : rect(r), isEnabled(true) {}
    virtual ~UIComponent() {}
    virtual void Draw(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f mousePos) = 0;
};

class Button : public UIComponent {
public:
    std::string text;
    sf::Color baseColor;

    Button(std::string t, float x, float y, float w, float h, sf::Color c)
        : UIComponent(sf::FloatRect({x, y}, {w, h})) {
        text = t;
        baseColor = c;
    }

    void Draw(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f mousePos) override {
        sf::Color color = isEnabled ? baseColor : GRAY_BG;
        if (isEnabled && rect.contains(mousePos)) {
            color.r = std::min(255, (int)color.r + 30);
            color.g = std::min(255, (int)color.g + 30);
            color.b = std::min(255, (int)color.b + 30);
        }
        
        DrawRectangleShapeRounded(window, rect, 10.f, color, BLACK_SUIT, 2.0f);
        
        sf::Text btnText(font, text, 20);
        btnText.setFillColor(sf::Color::White);
        btnText.setStyle(sf::Text::Bold);
        sf::FloatRect textBounds = btnText.getLocalBounds();
        btnText.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f});
        btnText.setPosition({rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f});
        window.draw(btnText);
    }

    bool IsClicked(sf::Vector2f mousePos, bool mousePressed) {
        return isEnabled && rect.contains(mousePos) && mousePressed;
    }
};

// Player Class & Inheritance
class Player {
protected:
    std::string name;          // ชื่อผู้เล่น
    std::vector<Card> hand;    // ไพ่ในมือ
    bool passed;               // สถานะว่าผ่านไปแล้วหรือยังในรอบนี้
public:
    Player(std::string n) : name(n), passed(false) {}
    virtual ~Player() = default;

    std::string getName() const { return name; }
    bool getPassed() const { return passed; }
    void setPassed(bool p) { passed = p; }

    const std::vector<Card>& getHand() const { return hand; }
    std::vector<Card>& getHandRef() { return hand; }

    void clearHand() { hand.clear(); }
    void addCard(const Card& card) { hand.push_back(card); } 

    // จัดเรียงไพ่ในมือจากค่าน้อยไปมาก
    void SortHand() {
        std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
            return a.getPower() < b.getPower();
        });
    }

    virtual bool IsHuman() const = 0; // กำหนดว่าเป็นคนหรือบอท
};

class HumanPlayer : public Player { // คลาสสำหรับคน
public:
    HumanPlayer(std::string n) : Player(n) {}
    bool IsHuman() const override { return true; }
};

class AIPlayer : public Player { // คลาสสำหรับบอท
public:
    AIPlayer(std::string n) : Player(n) {}
    bool IsHuman() const override { return false; }
};

// Game Engine Class
class GameEngine : public Board {
private:
    std::vector<Player*> players; // ลิสต์ผู้เล่นทั้งหมด
    int turnIdx;                  // ลำดับคนที่กำลังได้เล่น
    bool gameOver;                // สถานะจบเกม
    std::string winner;           // ชื่อผู้ชนะ
    std::string actionMsg;        // ข้อความแจ้งเตือนสถานการณ์ปัจจุบัน
    sf::Clock aiClock;            // นาฬิกาจับเวลาเพื่อหน่วงการตัดสินใจของบอท
    int lastPlayedIdx;            // เก็บว่าใครคือคนสุดท้ายที่ลงไพ่ได้

    // ย้ายเทิร์นไปคนถัดไป
    void NextTurn() {
        if (gameOver) return;

        // ถ้ามีใครไพ่หมดมือ ให้จบเกมทันที
        for (Player* p : players) {
            if (p->getHand().empty()) {
                gameOver = true;
                winner = p->getName(); // ประกาศชื่อคนที่ไพ่หมดเป็นผู้ชนะ
                actionMsg = winner + " has won the game!";
                return;
            }
        }

        // จบรอบนับว่าเหลือคนที่ยังไม่ผ่านกี่คน
        int activeCount = 0;
        for (Player* p : players) {
            if (!p->getPassed() && !p->getHand().empty()) activeCount++;
        }

        // ถ้าทุกคนผ่านหมดให้เริ่มรอบใหม่
        if (activeCount <= 1) {
            ClearBoard();
            for (Player* p : players) p->setPassed(false);
            turnIdx = lastPlayedIdx;
            actionMsg = players[turnIdx]->getName() + " won the trick. Lead any combo.";
            
            // ข้ามคนที่ไพ่หมดมือไปแล้ว
            while (players[turnIdx]->getHand().empty()) {
                turnIdx = (turnIdx + 1) % 4;
            }
            return;
        }

        // หาคนเล่นคนถัดไปที่ยังไม่ผ่านและยังมีไพ่ในมือ
        int nextIdx = turnIdx;
        while (true) {
            nextIdx = (nextIdx + 1) % 4;
            if (!players[nextIdx]->getHand().empty() && !players[nextIdx]->getPassed()) {
                break;
            }
        }
        turnIdx = nextIdx;
        aiClock.restart();
    }

public:
    // Constructor
    GameEngine() : turnIdx(0), gameOver(false), winner(""), actionMsg("Welcome to LormWong SLAVE!"), lastPlayedIdx(-1) {
        players.push_back(new HumanPlayer("You"));
        players.push_back(new AIPlayer("Bot 2"));
        players.push_back(new AIPlayer("Bot 3"));
        players.push_back(new AIPlayer("Bot 4"));
        DealCards(); // แจกไพ่ทันที
    }

    // Destructor
    ~GameEngine() {
        for (Player* p : players) delete p;
    }

    // Read-Only Parameter
    const std::vector<Player*>& getPlayers() const { return players; }
    int getTurnIdx() const { return turnIdx; }
    bool getGameOver() const { return gameOver; }
    std::string getWinner() const { return winner; }
    std::string getActionMsg() const { return actionMsg; }
    void setActionMsg(const std::string& msg) { actionMsg = msg; }
    void resetAIClock() { aiClock.restart(); }

    // รีเซ็ตสถานะเกมเพื่อเริ่มเล่นตาใหม่
    void ResetGame() {
        gameOver = false;
        winner = "";
        actionMsg = "Game restarted. 3 of Clubs starts!";
        lastPlayedIdx = -1;
        ClearBoard();
        for (Player* p : players) {
            p->clearHand();
            p->setPassed(false);
        }
        DealCards();
    }

    // สับไพ่และแจกไพ่ให้ทุกคน
    void DealCards() {
        std::vector<Card> deck; // สร้างไพ่ 52 ใบ
        for (std::string r : RANKS) {
            for (char s : SUITS) {
                // กำหนดค่าพลังของไพ่แต่ละใบให้ไม่ซ้ำกัน
                auto it_rank = std::find(RANKS.begin(), RANKS.end(), r);
                int rankVal = (int)std::distance(RANKS.begin(), it_rank);
                auto it_suit = std::find(SUITS.begin(), SUITS.end(), s);
                int suitVal = (int)std::distance(SUITS.begin(), it_suit);
                int power = (rankVal * 4) + suitVal;
                deck.push_back(Card(r, s, power, rankVal));
            }
        }

        // สับไพ่แบบสุ่ม
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);

        // แจกไพ่ทีละใบ วนให้ผู้เล่น 4 คน
        for (int i = 0; i < 52; i++) {
            players[i % 4]->addCard(deck[i]);
        }

        // จัดเรียงไพ่ในมือ และหาว่าใครมี 3 ดอกจิก จะได้เริ่มก่อน
        for (int i = 0; i < 4; i++) {
            players[i]->SortHand();
            for (const Card& c : players[i]->getHand()) {
                if (c.getRank() == "3" && c.getSuit() == 'C') {
                    turnIdx = i;
                    lastPlayedIdx = i;
                    actionMsg = players[i]->getName() + " starts with 3 of Clubs";
                }
            }
        }
        aiClock.restart();
    }

    // เมื่อผู้เล่นกดขอผ่าน
    void PassTurn(int pIdx) {
        if (pIdx != turnIdx || gameOver) return;
        players[pIdx]->setPassed(true);
        actionMsg = players[pIdx]->getName() + " passed.";
        NextTurn();
    }

    // เมื่อมีการเลือกลงไพ่ลงบนโต๊ะ
    void PlayCards(int pIdx, const std::vector<Card>& selected) {
        int count = (int)selected.size();
        std::string currentPlayType;
        
        // เช็คและสร้างข้อความแจ้งเตือนว่าลงไพ่รูปแบบไหน 1 คู่ ตอง 4
        if (!isTableEmpty()) {
            int reqCount = (int)getLastPlayed().size();
            if (reqCount == 1 && count == 3) currentPlayType = "triple (BOMB!)";
            else if (reqCount == 2 && count == 4) currentPlayType = "quad (BOMB!)";
            else currentPlayType = std::to_string(count) + " cards";
        } else {
            if (count == 1) currentPlayType = "single";
            else if (count == 2) currentPlayType = "pair";
            else if (count == 3) currentPlayType = "triple";
            else if (count == 4) currentPlayType = "quad";
        }

        actionMsg = players[pIdx]->getName() + " played " + currentPlayType;
        tableCards.push_back(selected); // นำไพ่ที่เลือกไปวางบนกองกลางโต๊ะ

        // ลบไพ่ที่ถูกลงไปแล้วออกจากมือผู้เล่น
        std::vector<Card> newHand;
        for (const Card& hc : players[pIdx]->getHand()) {
            bool found = false;
            for (const Card& pc : selected) {
                if (hc.getRank() == pc.getRank() && hc.getSuit() == pc.getSuit()) found = true;
            }
            if (!found) newHand.push_back(hc);
        }
        players[pIdx]->clearHand();
        for (const Card& nc : newHand) players[pIdx]->addCard(nc);

        lastPlayedIdx = pIdx; // บันทึกว่าคนนี้เป็นคนลงไพ่ล่าสุด
        NextTurn(); // เลื่อนเทิร์นถัดไป
    }

    // AI Auto-Play Logic
    void RunAITurn() {
        if (gameOver || players[turnIdx]->IsHuman()) return;
        if (aiClock.getElapsedTime().asSeconds() < 1.2f) return; // หน่วงเวลาคิด 1.2 วิ

        Player* ai = players[turnIdx];
        int reqCount = isTableEmpty() ? 0 : (int)getLastPlayed().size(); // เช็คจำนวนไพ่ที่ต้องลง
        int curPower = isTableEmpty() ? -1 : GetHighestPower(getLastPlayed()); // เช็คไพ่บนโต๊ะ

        // จัดไพ่ในมือบอทแยกตามตัวเลข ให้ง่ายในการหาคู่หรือตอง
        std::vector<std::vector<Card>> groups(13);
        for (const Card& c : ai->getHand()) {
            groups[c.getRankVal()].push_back(c);
        }

        // ถ้าโต๊ะว่างให้บอทลงไพ่ที่เล็กที่สุด 1 ใบ
        if (reqCount == 0) {
            ai->SortHand();
            PlayCards(turnIdx, { ai->getHand()[0] });
            return;
        }

        bool played = false;
        
        // ลูปหาไพ่จำนวนเท่ากันที่ใหญ่กว่าลงทับ
        for (auto& grp : groups) {
            if ((int)grp.size() >= reqCount) {
                int pwr = GetHighestPower(grp);
                if (pwr > curPower) {
                    std::vector<Card> toPlay(grp.begin(), grp.begin() + reqCount);
                    PlayCards(turnIdx, toPlay);
                    played = true;
                    break;
                }
            }
        }

        // ถ้าสู้ 1 ไม่ได้ ให้เล่นถ้ามีตองหรือ4
        if (!played) {
            if (reqCount == 1) {
                for (auto& grp : groups) {
                    if (grp.size() >= 3) {
                        std::vector<Card> toPlay(grp.begin(), grp.begin() + 3);
                        PlayCards(turnIdx, toPlay);
                        played = true; break;
                    }
                }
            }
            else if (reqCount == 2) {
                for (auto& grp : groups) {
                    if (grp.size() >= 4) {
                        std::vector<Card> toPlay(grp.begin(), grp.begin() + 4);
                        PlayCards(turnIdx, toPlay);
                        played = true; break;
                    }
                }
            }
        }

        if (!played) PassTurn(turnIdx); // ถ้าสู้ไม่ได้จริงๆ ให้ผ่าน
    }
    
    // ฟังก์ชันไพ่ที่เลือก ใบไหนมีพลังมากที่สุด
    int GetHighestPower(const std::vector<Card>& cards) const {
        if (cards.empty()) return -1;
        int maxP = -1;
        for (const auto& c : cards) {
            if (c.getPower() > maxP) maxP = c.getPower();
        }
        return maxP;
    }
};

enum class GameState { MENU, PLAYING }; // สถานะของฉาก

// ฟังก์ชันตรวจไพ่ที่ผู้เล่น
bool ValidateSelection(const std::vector<Card>& selected, int tableCount, int tablePower) {
    if (selected.empty()) return false;
    // หาว่าไพ่ทุกใบที่เลือกเป็นตัวเลขเดียวกันมั้ย
    std::string firstRank = selected[0].getRank();
    for (size_t i = 1; i < selected.size(); i++) {
        if (selected[i].getRank() != firstRank) return false;
    }

    // ถ้าโต๊ะว่าง อนุญาตให้ลงกี่ใบก็ได้
    if (tableCount == 0) return (selected.size() > 0 && selected.size() <= 4);

    int count = (int)selected.size();
    int currentPower = 0;
    for (const auto& c : selected) if (c.getPower() > currentPower) currentPower = c.getPower();

    // เช็คกฎการ SLAP
    if (tableCount == 1 && count == 3) return true; 
    if (tableCount == 2 && count == 4) return true;

    // เเช็คกฎปกติ
    return (count == tableCount && currentPower > tablePower);
}

int main() {
    // สร้างและตั้งค่าหน้าต่างเกม
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), 
        "LormWong SLAVE", 
        sf::State::Windowed, 
        settings
    );
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    GameEngine game; // สร้าง obj game
    GameState currentState = GameState::MENU;

    // ปุ่มกดสำหรับหน้า UI
    Button btnPlay("PLAY", SCREEN_WIDTH / 2.f + 50.f, SCREEN_HEIGHT - 220.f, 140.f, 45.f, sf::Color(70, 130, 180, 255));
    Button btnPass("PASS", SCREEN_WIDTH / 2.f - 190.f, SCREEN_HEIGHT - 220.f, 140.f, 45.f, sf::Color(200, 50, 50, 255));
    Button btnStart("START GAME", SCREEN_WIDTH / 2.f - 100.f, SCREEN_HEIGHT - 130.f, 200.f, 50.f, sf::Color(34, 139, 34, 255));
    Button btnRestart("PLAY AGAIN", SCREEN_WIDTH / 2.f - 100.f, SCREEN_HEIGHT / 2.f + 50.f, 200.f, 50.f, sf::Color(70, 130, 180, 255));

    // ลูปหลักของเกม
    while (window.isOpen()) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
        bool leftMousePressed = false;

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    leftMousePressed = true;
                }
            }
        }

        Card* hoveredCard = nullptr;
        Player* human = game.getPlayers()[0];

        // Game Logic Update
        if (currentState == GameState::MENU) {
            if (btnStart.IsClicked(mousePos, leftMousePressed)) {
                currentState = GameState::PLAYING;
                game.resetAIClock();
            }
        } 
        else if (currentState == GameState::PLAYING) {
            int curReq = game.isTableEmpty() ? 0 : (int)game.getLastPlayed().size();
            int curPower = game.isTableEmpty() ? -1 : game.GetHighestPower(game.getLastPlayed());

            // นับจำนวนไพ่ในมือของเรา
            std::vector<int> rankCounts(13, 0);
            for (const Card& c : human->getHandRef()) rankCounts[c.getRankVal()]++;

            // กำหนดให้ไพ่บนมือ ว่ากดเล่นได้ไหม
            for (Card& c : human->getHandRef()) {
                c.setPlayable(false);
                if (curReq == 0) { // โต๊ะว่าง ลงได้ทุกใบ
                    c.setPlayable(true);
                }
                else {
                    // เช็คเพื่อปลดล็อคไพ่ให้กดได้
                    int myGroupCount = rankCounts[c.getRankVal()];
                    int myGroupHighestPower = -1;
                    for (const Card& hc : human->getHandRef()) {
                        if (hc.getRankVal() == c.getRankVal() && hc.getPower() > myGroupHighestPower) {
                            myGroupHighestPower = hc.getPower();
                        }
                    }

                    if (curReq == 1) {
                        if (c.getPower() > curPower || myGroupCount >= 3) c.setPlayable(true);
                    }
                    else if (curReq == 2) {
                        if ((myGroupCount >= 2 && myGroupHighestPower > curPower) || myGroupCount >= 4) c.setPlayable(true);
                    }
                    else if (curReq == 3) {
                        if (myGroupCount >= 3 && myGroupHighestPower > curPower) c.setPlayable(true);
                    }
                    else if (curReq == 4) {
                        if (myGroupCount >= 4 && myGroupHighestPower > curPower) c.setPlayable(true);
                    }
                }
                if (!c.getIsPlayable()) c.setSelected(false); // ปลดเลือกไพ่ที่กดเล่นไม่ได้
            }

            // เช็คว่าเมาส์กำลัง Hover โดนไพ่ใบไหนอยู่
            for (auto it = human->getHandRef().rbegin(); it != human->getHandRef().rend(); ++it) {
                if (it->getRect().contains(mousePos) && it->getIsPlayable() && game.getTurnIdx() == 0) {
                    hoveredCard = &(*it);
                    break;
                }
            }

            // ระบบการคลิกไพ่และการกดปุ่มในเทิร์นของคน
            if (!game.getGameOver() && game.getTurnIdx() == 0) {
                // คลิกเพื่อเลือกไพ่ให้เด้งขึ้น
                if (leftMousePressed && hoveredCard != nullptr) {
                    hoveredCard->toggleSelected();
                }

                // ปุ่ม Play
                if (btnPlay.IsClicked(mousePos, leftMousePressed)) {
                    std::vector<Card> selected;
                    // ดึงไพ่ที่เด้งถูกเลือกไว้เข้าลิสเตรียมลง
                    for (const Card& c : human->getHandRef()) {
                        if (c.getIsSelected()) selected.push_back(c);
                    }

                    // เช็คกฎก่อนลง ถ้าผ่านก็ลงไพ่ ถ้าไม่ผ่านก็แจ้งเตือน
                    if (ValidateSelection(selected, curReq, curPower)) {
                        game.PlayCards(0, selected);
                    } else {
                        if (selected.empty()) {
                            game.setActionMsg("Oops, you need to pick some cards!");
                        } else {
                            game.setActionMsg("That move is not allowed, check the rules.");
                            for (Card& c : human->getHandRef()) c.setSelected(false); // รีเซ็ตการเลือกไพ่
                        }
                    }
                }

                // ปุ่มผ่าน
                if (btnPass.IsClicked(mousePos, leftMousePressed) && !game.isTableEmpty()) {
                    for (Card& c : human->getHandRef()) c.setSelected(false);
                    game.PassTurn(0);
                }
            }

            // ถ้าเกมจบ รอให้กดปุ่มเล่นใหม่ ถ้ายังไม่จบ ให้เทิร์นบอทเดิน
            if (game.getGameOver()) {
                if (btnRestart.IsClicked(mousePos, leftMousePressed)) {
                    game.ResetGame();
                }
            } else {
                game.RunAITurn();
            }
        }

        // Rendering
        window.clear(GREEN_FELT);
        
        if (currentState == GameState::MENU) {
            sf::FloatRect rulesRect({SCREEN_WIDTH / 2.f - 350.f, 80.f}, {700.f, 560.f});
            DrawRectangleShapeRounded(window, rulesRect, 15.f, DARK_BG, GOLD_H, 3.0f);

            sf::Text titleText(font, "LormWong SLAVE", 40);
            titleText.setFillColor(GOLD_H);
            titleText.setStyle(sf::Text::Bold);
            sf::FloatRect titleBounds = titleText.getLocalBounds();
            titleText.setPosition({SCREEN_WIDTH / 2.f - titleBounds.size.x / 2.f, 110.f});
            window.draw(titleText);

            std::string rulesStr = 
                "HOW TO PLAY:\n\n"
                "1. Rank: 3 is lowest, 2 is highest.\n"
                "   (Suit: Club < Diamond < Heart < Spade)\n\n"
                "2. Player with 3 of Clubs starts first.\n\n"
                "3. You must match the number of cards on the table.\n\n"
                "4. Your cards must be higher rank than the table.\n\n"
                "5. SLAP rule: \n"
                "   - 3 of a kind beats any single card.\n"
                "   - 4 of a kind beats any pair.\n\n"
                "6. Last person to play starts the new round.\n\n"
                "7. First to empty their hand wins!";
            sf::Text rulesText(font, rulesStr, 18);
            rulesText.setFillColor(sf::Color::White);
            rulesText.setLineSpacing(1.2f);
            rulesText.setPosition({SCREEN_WIDTH / 2.f - 300.f, 175.f});
            window.draw(rulesText);

            btnStart.isEnabled = true;
            btnStart.Draw(window, font, mousePos);
        }
        else if (currentState == GameState::PLAYING) {
            
            std::vector<sf::Vector2f> botPos = {
                { SCREEN_WIDTH / 2.f, 60.f },
                { 120.f, SCREEN_HEIGHT / 2.f - 50.f },
                { SCREEN_WIDTH - 120.f, SCREEN_HEIGHT / 2.f - 50.f }
            };
            for (int i = 0; i < 3; i++) {
                int pIdx = i + 1;
                sf::Color color = GOLD_H; if (game.getTurnIdx() != pIdx) color = sf::Color::White;
                std::string status = game.getPlayers()[pIdx]->getPassed() ? 
                    "PASSED" : std::to_string(game.getPlayers()[pIdx]->getHand().size()) + " Cards";

                sf::FloatRect botCardRec({botPos[i].x - 40.f, botPos[i].y - 55.f}, {80.f, 110.f});
                DrawRectangleShapeRounded(window, botCardRec, 6.f, DARK_BG, sf::Color::White, 2.0f);

                std::string nameTxt = game.getPlayers()[pIdx]->getName() + ": " + status;
                sf::Text botText(font, nameTxt, 20);
                botText.setFillColor(color);
                botText.setStyle(sf::Text::Bold);
                sf::FloatRect textBounds = botText.getLocalBounds();
                botText.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f});
                botText.setPosition({botPos[i].x, botPos[i].y + 75.f});
                window.draw(botText);
            }

            if (!game.isTableEmpty()) {
                std::vector<Card> play = game.getLastPlayed();
                int pSize = (int)play.size();
                for (int i = 0; i < pSize; i++) {
                    play[i].Draw(window, font, (float)(SCREEN_WIDTH / 2 - 50 + (i * 105) - (pSize * 105 / 2)), (float)(SCREEN_HEIGHT / 2 - 120), false, true);
                }
            }

            sf::RectangleShape msgBg(sf::Vector2f({700.f, 45.f}));
            msgBg.setFillColor(MSG_BG);
            msgBg.setOrigin({350.f, 22.5f});
            msgBg.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 42.5f});
            window.draw(msgBg);

            sf::Text actionText(font, game.getActionMsg(), 24);
            actionText.setFillColor(GOLD_H);
            sf::FloatRect actBounds = actionText.getLocalBounds();
            actionText.setOrigin({actBounds.position.x + actBounds.size.x / 2.0f, actBounds.position.y + actBounds.size.y / 2.0f});
            actionText.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 40.f});
            window.draw(actionText);

            if (!game.getGameOver()) {
                sf::RectangleShape turnBg(sf::Vector2f({360.f, 45.f}));
                turnBg.setFillColor(MSG_BG);
                turnBg.setOrigin({180.f, 22.5f});
                turnBg.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 102.5f});
                window.draw(turnBg);
                if (game.getTurnIdx() == 0) {
                    sf::Text turnText(font, "YOUR TURN!", 24);
                    turnText.setFillColor(RED_SUIT); 
                    turnText.setStyle(sf::Text::Bold);
                    sf::FloatRect tBounds = turnText.getLocalBounds();
                    turnText.setOrigin({tBounds.position.x + tBounds.size.x / 2.0f, tBounds.position.y + tBounds.size.y / 2.0f});
                    turnText.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 100.f});
                    window.draw(turnText);
                }
                else {
                    std::string tMsg = "Waiting for " + game.getPlayers()[game.getTurnIdx()]->getName() + "...";
                    sf::Text turnText(font, tMsg, 20);
                    turnText.setFillColor(GRAY_TEXT);
                    sf::FloatRect tBounds = turnText.getLocalBounds();
                    turnText.setOrigin({tBounds.position.x + tBounds.size.x / 2.0f, tBounds.position.y + tBounds.size.y / 2.0f});
                    turnText.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 100.f});
                    window.draw(turnText);
                }
            }

            int hSize = (int)human->getHand().size();
            int handW = hSize * 45 + 50;
            int startX = SCREEN_WIDTH / 2 - handW / 2;
            for (int i = 0; i < hSize; i++) { 
                bool isHov = (&human->getHandRef()[i] == hoveredCard);
                human->getHandRef()[i].Draw(window, font, (float)(startX + (i * 45)), (float)(SCREEN_HEIGHT - 150), isHov);
            }

            btnPlay.isEnabled = (game.getTurnIdx() == 0 && !game.getGameOver());
            btnPass.isEnabled = (game.getTurnIdx() == 0 && !game.isTableEmpty() && !game.getGameOver());
            btnPlay.Draw(window, font, mousePos);
            btnPass.Draw(window, font, mousePos);

            if (game.getGameOver()) {
                sf::RectangleShape endBg(sf::Vector2f({static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)}));
                endBg.setFillColor(sf::Color(0, 0, 0, 210));
                window.draw(endBg);

                std::string winMsg = game.getWinner() + " WINS THE GAME!";
                sf::Text winText(font, winMsg, 50);
                winText.setFillColor(GOLD_H);
                winText.setStyle(sf::Text::Bold);
                sf::FloatRect wBounds = winText.getLocalBounds();
                winText.setOrigin({wBounds.position.x + wBounds.size.x / 2.0f, wBounds.position.y + wBounds.size.y / 2.0f});
                winText.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 60.f});
                window.draw(winText);

                btnRestart.isEnabled = true;
                btnRestart.Draw(window, font, mousePos);
            }
        }
        window.display();
    }
    return 0;
}