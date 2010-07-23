import simplexquery as sxq


print("""
['execute' xquery code]\
""")
print(sxq.execute("""<user>{"Taro"}</user>"""))

print("""
['execute' xquery code with context xml]\
""")
print(sxq.execute("<html><body>{string(/author/name)}</body></html>",
                  "<author><name>Taro</name></author>"))

print("""
['execute' xquery code with uri -> xml resolver callback]\
""")
print(sxq.execute('<body>{doc(/root/@href)/name}</body>', 
                  "<root href='/bar.xml'/>", 
                  lambda uri: "<name>%s</name>" % uri))

class Resolver(object):
    def __call__(self, uri):
        print(uri)
        return "<name>Jiro</name>"
    pass
print(sxq.execute('doc("foo.xml")/name', resolver=Resolver()))

print("""
['execute' returns None if fails by anyway]\
""")
print(sxq.execute("<user>")) 
print(sxq.execute("<user>{string(/name)}</user>", "<name>Taro<name>")) 



print("""
['execute_all' returns a list of multiple results]\
""")
print(repr(sxq.execute_all("/user/name",
                           "<user><name>Taro</name><name>Jiro</name></user>")))

print("""
['execute_all' raise ValueError if xquery execution is failed]\
""")
try:
    sxq.execute_all("/user'")
    pass
except ValueError, ex:
    print(ex)
    pass
try:
    sxq.execute_all('doc("foo.xml")/name', resolver=lambda uri : "non xml")
    pass
except ValueError, ex:
    print(ex)
    pass

print("""
['execute_all' raise TypeError if arg has invalid type]\
""")
try:
    sxq.execute_all(0)
    pass
except TypeError, ex:
    print(ex)
    pass
try:
    sxq.execute_all("", "", 0)
    pass
except TypeError, ex:
    print(ex)
    pass

try:
    sxq.execute_all('doc("foo.xml")/name', resolver=lambda : "abc")
    pass
except TypeError, ex:
    print(ex)
    pass
try:
    sxq.execute_all('doc("foo.xml")/name', resolver=lambda a : None)
    pass
except TypeError, ex:
    print(ex)
    pass

print("""
['versions' returns version tuple]\
""")
print(sxq.versions())

print("""
[end]\
""")
