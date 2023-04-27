def main() -> None:

    balance = float(input("Enter the initial balance: "))
    deposit = []
    while True:
        transaction = float(input("Enter the transaction amount: "))
        if transaction == 0:
            break
        else:
            balance += transaction
            if transaction > 0:
                deposit.append(transaction)
    print(f"Balance: {balance}\nAverage deposit amount: {sum(deposit) / len(deposit)}")
    del deposit


if __name__ == '__main__':
    main()
