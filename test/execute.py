import unittest
import simplexquery as sxq

class execute(unittest.TestCase):
    def test_with_xquery(t):
        t.assertEqual(
            sxq.execute("""<user>{"Taro"}</user>"""),
            "<user>Taro</user>")
        pass
    
    def test_with_xquery_context(t):
        t.assertEqual(
            sxq.execute("<html><body>{string(/author/name)}</body></html>",
                        "<author><name>Taro</name></author>"),
            "<html><body>Taro</body></html>")
        pass

    def test_with_xquery_conext_resolver(t):
        t.assertEqual(
            sxq.execute('<body>{doc(/root/@href)/name}</body>',
                        "<root href='/bar.xml'/>",
                        lambda uri: "<name>%s</name>" % uri),
            "<body><name>/bar.xml</name></body>")
        pass
    
    class ResolverClass(object):
        def __call__(self, uri):
            return "<name>Jiro</name>"
        pass
    def test_with_xquery_resolver(t):
        t.assertEqual(
            sxq.execute('doc("foo.xml")/name', resolver=t.ResolverClass()),
            "<name>Jiro</name>")
        pass
    
    def test_returns_none_if_execute_fails(t):
        t.assertEqual(
            sxq.execute("<user>"),
            None)
        t.assertEqual(
            sxq.execute("<user>{string(/name)}</user>", "<name>Taro<name>"),
            None)
        pass
    pass


if __name__ == "__main__": unittest.main()
    
