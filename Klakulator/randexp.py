from random import randint

op = ['+', '-', '*', '/', '^']
opcount = len(op)

exp = f'{randint(1, 100)}'
iter = randint(1, 5)
for i in range(iter):
    exp += f' {op[randint(0, opcount - 1)]} '
    
    chance = randint(1, 10)
    if chance < 4:
        exp += '-'
        
    exp += f'{randint(1, 100)}'
    
print(exp)