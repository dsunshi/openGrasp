/*
 * Copyright (c) 2015 David Sunshine, <http://sunshin.es>
 * 
 * This file is part of openGrasp.
 * 
 * openGrasp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * openGrasp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with openGrasp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include "sm.h"
#include "../shoe/shoe.h"
#include "../hand/hand.h"
#include "../thorp/thorp.h"
#include "../histogram/histogram.h"
#include "../truecount/truecount.h"

#define NUM_DECKS          1
#define MAX_ALLOWED_SPLIT  5
#define STARTING_WALLET    200000
#define BET_INCREMENT      1

#define ROUNDS_TO_PLAY     1000000

#define GET_DEALER_UPCARD(hand) get_card_value( (hand), 1 )

/* Global Variables */
static Shoe       *shoe;
static Hand       *dealer_hand;
static Hand       *player_hands[MAX_ALLOWED_SPLIT];
static gint        bets[MAX_ALLOWED_SPLIT];
static gint        num_hands;
static Histogram  *histogram;
static TrueCount  *true_count;
static gint        wallet;

static gint        current_round;

/* Global Functions */
static void  shuffle_notification (void);
static gint8 get_card_from_shoe   (void);
static void  place_bet            (gint8 hand);

static void shuffle_notification (void)
{
    reset_true_count( true_count );
    reset_histogram(  histogram  );
}

static gint8 get_card_from_shoe()
{
    gint8 card;
    
    card = get_card( shoe );
    
    update_histogram(  histogram,  card );
    update_true_count( true_count, card );
    
    return card;
}

static void place_bet ( gint8 hand )
{
    gint amount = BET_INCREMENT;

	if (wallet > amount)
	{
		wallet     -= amount;
		bets[hand] += amount;
	}
	else
	{
        /* Player out of money */
	}
}

StateFunc init (void)
{
    gint index;
    
    /* Shoe */
    shoe = create_shoe( NUM_DECKS );
    SHOE_SET_SHUFFLE_CALLBACK(shoe, shuffle_notification);
    
    /* Player */
    for (index = 0; index < MAX_ALLOWED_SPLIT; index++)
    {
        player_hands[index] = create_hand();
    }
    num_hands = 1;
    
    /* Dealer */
    dealer_hand = create_hand();
    
    /* Histogram */
    histogram = create_histogram( NUM_DECKS );
    
    /* True Count */
    true_count = create_true_count();
    
    /* Wallet */
    wallet = STARTING_WALLET;
    
    /* Number of rounds to play */
    current_round = 0;
    
    /* After init the next state is: shuffle */
    return FUNCTION_TO_STATE(shuffle);
}

StateFunc shuffle (void)
{
    shuffle_shoe( shoe );
    
    /* After shuffle the next state is: betting */
    return FUNCTION_TO_STATE(betting);
}

StateFunc betting (void)
{
    /* 1. Get the amount to bet */
    
    /* 2. Place the bet */
    place_bet(0);
    
    /* After betting the next state is: dealing */
    return FUNCTION_TO_STATE(dealing);
}

StateFunc dealing (void)
{
    gint8 current_card;
    gint  index;
    
    current_card = get_card_from_shoe();

	/* Deal two cards to the player and dealer */
	for (index = 0; index < 2; index++)
	{
        /* Player */
		current_card = get_card_from_shoe(); 
		receive_card(player_hands[0], current_card);
		
        /* Dealer */
		current_card = get_card_from_shoe();
		receive_card(dealer_hand, current_card);
	}
    
    /* After dealing the next state is: players */
    return FUNCTION_TO_STATE(players);
}

StateFunc players (void)
{
	Action  action;
	gint8   upcard;
	gint8   current_card;
	gint    current_hand;
	gint    bet;
    gint    value;
	gint    bust_odds;
    
	upcard = GET_DEALER_UPCARD(dealer_hand);
	
	for (current_hand = 0; current_hand < num_hands; current_hand++)
	{
		do
		{
            /* Thorp Action */
			action    = get_thorp_action( player_hands[current_hand], upcard );
            value     = get_hand_value( player_hands[current_hand] );
            bust_odds = get_bust_odds( histogram, value );

			switch (action)
			{
				case HIT:
					current_card = get_card_from_shoe(); 
					receive_card( player_hands[current_hand], current_card );
					break;
				case SPLIT:
					/* Split the current hand into two */
                    if (num_hands < MAX_ALLOWED_SPLIT)
                    {
                        current_card = get_card_value( player_hands[current_hand], 1 );
                        remove_last_card( player_hands[current_hand] );
                        receive_card( player_hands[num_hands], current_card );
                        
                        /* Deal another card to each hand */
                        current_card = get_card_from_shoe();
                        receive_card( player_hands[current_hand], current_card );
                        
                        current_card = get_card_from_shoe();
                        receive_card( player_hands[num_hands], current_card );
                        /* Allocate the new hand as used */
                        num_hands++;

                        bet = bets[current_hand];
                        place_bet(current_hand);
                    }
                    else
                    {
                        /* Cannot split because the no more are allowed, so HIT instead */
                        current_card = get_card_from_shoe(); 
                        receive_card( player_hands[current_hand], current_card );
                    }
					break;
				case DOUBLE:
					bet = bets[current_hand];
					place_bet(current_hand);
					
					current_card = get_card_from_shoe();
                    receive_card( player_hands[current_hand], current_card );

					/* Player is forced to stand after receiving one card */
					action = STAND;
					break;
				case STAND:
					break;
				default:
					break;
			}
            
		} while (action != STAND);
    }

    /* After players the next state is: dealer */
    return FUNCTION_TO_STATE(dealer);
}

StateFunc dealer (void)
{
	gint   value;
	gint8  current_card;
	
	value = get_hand_value(dealer_hand);
	
	while (value <= 17)
	{
		current_card = get_card_from_shoe(); 
		receive_card(dealer_hand, current_card);
		
		value = get_hand_value(dealer_hand); 
	}
    
    /* After dealer the next state is: payout */
    return FUNCTION_TO_STATE(payout);
}

StateFunc payout (void)
{
    StateFunc next = NULL;
    
    gint  current_hand;
	gint  dealer_value;
	gint  player_value;
	
	dealer_value = get_hand_value(dealer_hand);
	
	for (current_hand = 0; current_hand < num_hands; current_hand++)
	{
		player_value = get_hand_value( player_hands[current_hand] );
		
		if (player_value == BUST)
		{
			/* Player Busted */	
            
            /* House takes the bet */
            bets[current_hand] = 0;
		}
		else if (dealer_value == BUST)
		{
			/* Dealer Busted */	
            
            /* Player wins their bet amount ... */
			wallet += bets[current_hand];
            /* .. and receives their bet back   */
            wallet += bets[current_hand];
            /* Now there is no bet on the table */
            bets[current_hand] = 0;
		}
		else if (player_value == dealer_value)
		{
			/* Push */	
			
            /* Player ONLY receives their bet back   */
            wallet += bets[current_hand];
            /* Now there is no bet on the table */
            bets[current_hand] = 0;
		}
		else if (player_value > dealer_value)
		{
			/* Player beat Dealer */
			
            /* Player wins their bet amount ... */
			wallet += bets[current_hand];
            /* .. and receives their bet back   */
            wallet += bets[current_hand];
            /* Now there is no bet on the table */
            bets[current_hand] = 0;
		}
		else
		{
			/* Player lost to Dealer */
            
            /* House takes the bet */
            bets[current_hand] = 0;
		}
		
        /* Reset the current hand and current bet */
		reset_hand( player_hands[current_hand] );
	}

	/* Reset the Dealers hand */	
	reset_hand( dealer_hand) ;

	/* Reset the number of hands in case there were any splits */	
	num_hands = 1;
    
    if ( SHOE_GET_REMAINING_CARDS(shoe) <  30 )
    {
        next = FUNCTION_TO_STATE(shuffle);
    }
    else
    {
        next = FUNCTION_TO_STATE(betting);
    }
    
    if (wallet <= BET_INCREMENT)
    {
        /* Player is out of money to continue */
        next = FUNCTION_TO_STATE(finished);
    }
    else
    {
        /* Normal game play */
        current_round++;
    }
    
    /* After payout the next state is: betting or shuffle */
    return next;
}

StateFunc finished (void)
{
    gint index;
    
    /* Free all of the allocated memory */
    destroy_shoe( shoe );
    destroy_hand( dealer_hand );
    
    for (index = 0; index < MAX_ALLOWED_SPLIT; index++)
    {
        destroy_hand( player_hands[index] );
    }
    
    destroy_true_count( true_count );
    destory_histogram(  histogram  );
    
    /* Result Summary */
    printf("Current Round:  \t%d\n", current_round);
    printf("Starting Wallet:\t%d\n", STARTING_WALLET);
    printf("Current Wallet: \t%d\n", wallet);
    
    exit(EXIT_SUCCESS);
    
    return NULL;
}

int main (void)
{
    StateFunc statefunc = FUNCTION_TO_STATE(init);
    
    do
    {
        /* Main state machine loop */
        statefunc = FUNCTION_TO_STATE( (*statefunc)() );
    } while (current_round < ROUNDS_TO_PLAY);
    
    /*
     * If we reach this point it is because we hit
     * the ROUNDS_TO_PLAY limit and did not run out of money, so
     * it is still necessary to perform the required "clean-up" steps.
     */
    (void)finished();

    return 0;
}