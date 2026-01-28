import os, time, random, sys, select

width, height = 30, 15
box_x, box_w = width//2, 6
candy_x, candy_y = random.randint(0, width-1), 0
score, life = 0, 5

def clear():
    os.system("cls" if os.name == "nt" else "clear")

def key_pressed():
    return select.select([sys.stdin], [], [], 0)[0]

while life > 0:
    clear()
    for y in range(height):
        line = ""
        for x in range(width):
            if y == candy_y and x == candy_x:
                line += "O"
            elif y == height-2 and box_x <= x < box_x + box_w:
                line += "#"
            else:
                line += " "
        print(line)
    print(f"Score: {score}  Life: {life}  (a=left, d=right, q=quit)")

    start = time.time()
    while time.time() - start < 0.25:
        if key_pressed():
            ch = sys.stdin.read(1)
            if ch == "q":
                sys.exit()
            if ch == "a" and box_x > 0:
                box_x -= 1
            if ch == "d" and box_x + box_w < width:
                box_x += 1

    candy_y += 1
    if candy_y >= height-2:
        if box_x <= candy_x < box_x + box_w:
            score += 10
        else:
            life -= 1
        candy_y = 0
        candy_x = random.randint(0, width-1)

clear()
print("GAME OVER! Final Score:", score)
