/*! Deck Class */

#ifndef __ROBORALLY_DECK_HH__
#define __ROBORALLY_DECK_HH__

enum
{
    FORWARD, BACKWARD, TURN
};

struct Card
{
    int type;
    int number;
    
};

class Deck
{
public:
    /** Everything to do with Cards.
     *
     */
    
    Deck();
    ~Deck();
    
    /** Gives player a card.
     *
     */
    const Card pop();
    
private:
    /** How many cards are in deck and of which type
     *
     *
     */
    std::vector<Card> deck_;
    
};


#endif // __ROBORALLY_DECK_HH__
