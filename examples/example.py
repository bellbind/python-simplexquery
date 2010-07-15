import simplexquery as sxq


tpl = """
<html>
<body>
<h1>
{ string(/author/name) }
</h1>
</body>
</html>
"""
xml = """
<author>
  <name>Taro</name>
</author>
"""
print(sxq.execute(tpl, xml))

print(sxq.execute("""<user>{"Taro"}</user>"""))

# return None if wrong args 
print(sxq.execute("<user>")) 
print(sxq.execute("<user>{string(/name)}</user>", "<name>Taro<name>")) 


# multiple results
print(repr(sxq.execute_all("/user/name",
                           "<user><name>Taro</name><name>Jiro</name></user>")))
# raise ValueError if wrong args
try:
    sxq.execute_all("/user'")
    pass
except ValueError, ex:
    print(ex)
    pass

def resolver(uri):
    print(uri)
    return "<name>Jiro</name>"
print(sxq.execute_all('doc("foo.xml")/name', resolver=resolver))
