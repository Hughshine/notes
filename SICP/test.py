# def print_sums(n):
#     print(n)
#     def next_sum(k):
#         return print_sums(n+k)
#     return next_sum
# print_sums(1)(3)(0)(0)

lst = [1,2,3]

l = iter(lst)
# lst[0] = 4 # iter指向lst 
print(next(l))
print(next(l))
print(next(l))

def make_withdraw(balance):
    def withdraw(amount):
        nonlocal balance
        if amount > balance:
            return 'Insufficient funds'
        balance = balance - amount
        return balance
    return withdraw

# 账户 = make_withdraw(100)

# print(账户(10))

def plus_minus(x):
    yield x
    yield -x

t = plus_minus(3)
print(next(t))
print(next(t))
# print(next(t)) # 已经结束了

# 