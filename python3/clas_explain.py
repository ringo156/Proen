class Spam:
    val = 100
    def ham(self):
        self.egg('call method')

    def egg(self,msg):
        print(msg)
        print(("{0}".format(self.val)))

spam = Spam()
spam.ham()
