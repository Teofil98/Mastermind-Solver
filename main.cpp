#include <stdio.h>
#include <math.h>
#include <set>

int generate_initial_guess(int num_digits)
{
	int guess = 0;
	// fill first half with 1s
	for(int i = 0; i < num_digits/2; i++) {
		guess += 1;
		guess *= 10;
	}

	// fill second half with 2s
	for(int i = num_digits/2; i < num_digits; i++) {
		guess += 2;
		guess *= 10;
	}

	// remove extra 0 at the end
	guess /= 10;

	return guess;
}

bool valid(int number, int guess, int black_pegs, int white_pegs, int num_digits) {
	int digits_number[10] = {0};
	int digits_guess[10] = {0};
	int k = 0;

	while(number != 0 && guess != 0) {
		digits_number[k] = number % 10;
		digits_guess[k] = guess % 10;

		k++;
		number /= 10;
		guess /= 10;
	}

	int kn = k;
	while(number != 0) {
		digits_number[kn] = number % 10;
		kn++;
		number /= 10;
	}

	int kg = k;
	while(guess != 0) {
		digits_guess[kg] = guess % 10;
		kg++;
		guess /= 10;
	}

	int number_black_pegs = 0;
	for(int i = 0; i < num_digits; i++) {
		if(digits_number[i] == digits_guess[i])
			number_black_pegs++;
	}

	if(number_black_pegs != black_pegs)
		return false;


	int number_white_pegs = 0;
	for(int i = 0; i < num_digits; i++) {
        if(digits_guess[i] == digits_number[i])
            continue;
		for(int j = 0; j < num_digits; j++) {
			if(i != j &&
                   // digits_number[j] != digits_guess[j] && // don't count if it has already been matched with a black peg
                    //digits_number[i] != digits_guess[i] && // don't count if it has already been matched with a black peg
                    digits_guess[i] == digits_number[j]) {
				number_white_pegs++;
                break;
			}
		}
	}

	if(number_white_pegs != white_pegs)
		return false;

	return true;

}

bool heuristic(int score, int ans_size) {
    static int turns = 0;
    if(turns < 1 && score < ans_size/8) {
        turns++;
        return true;
    }
    return false;
}

int main()
{
	int num_digits;
	printf("Number of digits: ");
	scanf("%d", &num_digits);

	std::set<int> possible_answers;
	int max_value = pow(10, num_digits);
	for(int i = 0; i < max_value; i++) {
		possible_answers.insert(i);
	}

	int guess = generate_initial_guess(num_digits);
	printf("Initial guess: %d\n", guess);
	while(possible_answers.size() > 1) {
		// Number of pegs of guess
		int black_pegs, white_pegs;
		printf("Black pegs: ");
		scanf("%d", &black_pegs);
		printf("white pegs: ");
		scanf("%d", &white_pegs);

		// Remove non-valid answers
		auto it = std::begin(possible_answers);
		while(it != std::end(possible_answers)) {
			if(!valid(*it, guess, black_pegs, white_pegs, num_digits)) {
				it = possible_answers.erase(it);
			} else {
				it++;
			}
		}

		// check all possible answers and see the minimal worst value
		int min_guess;
		int min_score = max_value;
		for(int num = 0; num < max_value; num++) {
			int max_local_score = 0;
			for(int wp = 0; wp <= num_digits; wp++) {
				for(int bp = 0; bp <= num_digits; bp++) {
					if(wp + bp <= num_digits) {
						int local_score = 0;
						for(int ans : possible_answers) {
							if(valid(ans, num, bp, wp, num_digits))
								local_score++;
						}

						if(local_score > max_local_score)
							max_local_score = local_score;

					}
				}
			}

			if(max_local_score > 0 && max_local_score < min_score) {
				min_score = max_local_score;
				guess = num;
                if(heuristic(max_local_score, possible_answers.size()))
                    break;
			}
		}

		// make new guess
		printf("New guess: %d\n", guess);

	}

    printf("Solution: %d\n", *possible_answers.begin());

	return 0;
}
