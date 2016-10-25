class Base:
    basevalue = "base"
    def spam(self):
        print("Base.spam()")

    def ham(self):
        print("ham")

class Derived(Base):
    def spam(self):
        print ("Derived.spam()")
        self.ham()

derived = Derived()
print("{0}".format(derived.basevalue))
derived.ham()
derived.spam()
