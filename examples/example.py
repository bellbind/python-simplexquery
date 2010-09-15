import traceback
import simplexquery as sxq

def prompt(title):
    print("\n" + title)
    pass


prompt("['execute' xquery code]")
print(sxq.execute("""<user>{"Taro"}</user>"""))


prompt("['execute' xquery code with context xml]")
print(sxq.execute("<html><body>{string(/author/name)}</body></html>",
                  "<author><name>Taro</name></author>"))


prompt("['execute' xquery code with uri -> xml resolver callback]")
print(sxq.execute('<body>{doc(/root/@href)/name}</body>', 
                  "<root href='/bar.xml'/>", 
                  lambda uri: "<name>%s</name>" % uri))

class Resolver(object):
    def __call__(self, uri):
        print(uri)
        return "<name>Jiro</name>"
    pass
print(sxq.execute('doc("foo.xml")/name', resolver=Resolver()))


prompt("['execute' returns None if fails by anyway]")
print(sxq.execute("<user>")) 
print(sxq.execute("<user>{string(/name)}</user>", "<name>Taro<name>")) 


prompt("['execute_all' returns a list of multiple results]")
print(repr(sxq.execute_all("/user/name",
                           "<user><name>Taro</name><name>Jiro</name></user>")))


prompt("['execute_all' raise ValueError if xquery execution is failed]")
try: sxq.execute_all("/user'")
except ValueError: traceback.print_exc()

try: sxq.execute_all('doc("foo.xml")/name', resolver=lambda uri : "non xml")
except ValueError: traceback.print_exc()


prompt("['execute_all' raise TypeError if arg has invalid type]")
try: sxq.execute_all(0)
except TypeError: traceback.print_exc()

try: sxq.execute_all("", "", 0)
except TypeError: traceback.print_exc()

try: sxq.execute_all('doc("foo.xml")/name', resolver=lambda : "abc")
except TypeError: traceback.print_exc()

try: sxq.execute_all('doc("foo.xml")/name', resolver=lambda a : None)
except TypeError: traceback.print_exc()


prompt("['versions' returns version tuple]")
print(sxq.versions())

prompt("[end]")
