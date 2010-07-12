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
