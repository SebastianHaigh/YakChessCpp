#ifndef YAK_MOVE_H_
#define YAK_MOVE_H_

#include "bitboard.h"
#include "pieces.h"
#include <memory>

using pieces::Type;
using pieces::Colour;

namespace move {

class Move {
    protected:
        virtual std::unique_ptr<Move> 
    public:
        virtual void make();
}

class MoveDecorator: public Move {

}

class BaseMove {
    private:
        Bitboard from;
        Bitboard to;
        pieces::Type type;
        pieces::Colour colour;
    public:
        BaseMove(Bitboard from, Bitboard to, Type type, Colour colour);

};

class QuietMove: public BaseMove {
    
};

class CaptureMove: public BaseMove {

};

class Promotion {

};
    
} // namespace move


#endif // YAK_MOVE_H_