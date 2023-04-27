import random


def throw_until(x):
    rolls = 0
    while True:
        dice1 = random.randint(1, 6)
        dice2 = random.randint(1, 6)
        rolls += 1
        if dice1 + dice2 == x:
            print(f"Dice1: {dice1}\tDice2: {dice2}")
            return rolls


if __name__ == '__main__':
    while True:
        user_input = input("Enter sum of dice: ")
        try:
            sum_value = int(user_input)
            if 2 <= sum_value <= 12:
                break
            else:
                print("Sum must be between 2 and 12 inclusive\n")
        except ValueError:
            print("Sum must be an integer\n")

    roll_count = throw_until(sum_value)

    print(f"Dice are rolled {roll_count} {'time' if roll_count == 1 else 'times'} to get the sum {user_input}\n")
