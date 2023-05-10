def main() -> None:

    red = 0
    grn = 0
    for i in range(8):
        color = input(f"Enter the traffic light color R, G, Y for car {i + 1}: ")
        if color.upper() == 'R':
            red += 1
        elif color.upper() == 'Y':
            print("Ready to pass")
        elif color.upper() == 'G':
            grn += 1
        else:
            print("Invalid traffic light color!")
    print(f"\nNumber of cars stopped: {red}")
    print(f"Percentage of cars passed: {grn / 8 * 100}%")


if __name__ == '__main__':
    main()
