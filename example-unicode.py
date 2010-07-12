# -*- coding: utf-8 -*-
import simplexquery as sxq


tpl = u"""
<html>
<body>
<h1>
{ string(/author/name) }
</h1>
</body>
</html>
"""
xml = u"""
<author>
  <name>\u592a\u90ce</name>
</author>
"""
print(sxq.execute(tpl, xml))

print(sxq.execute(u"""<user>{"\u592a\u90ce"}</user>"""))
