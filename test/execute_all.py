import unittest
import simplexquery as sxq

class execute_all(unittest.TestCase):
    def test_with_code_which_result_is_sequence(t):
        results = sxq.execute_all(
            "data(/user/name)",
            "<user><name>Taro</name><name>Jiro</name></user>")
        expects = ["Taro", "Jiro"]
        for index, expected in enumerate(expects):
            t.assertEqual(results[index], expected)
            pass
        pass
    
    def test_with_syntax_error_xquery(t):
        t.assertRaises(
            ValueError,
            lambda : sxq.execute_all("/user'"))
        pass
    
    def test_with_non_xml_context(t):
        t.assertRaises(
            ValueError,
            lambda : sxq.execute_all("/user", "non xml"))
        pass
    
    def test_with_non_xml_resolver_result(t):
        t.assertRaises(
            ValueError,
            lambda : sxq.execute_all('doc("foo.xml")/name',
                                     resolver=lambda uri : "non xml"))
        pass
            
    def test_with_non_unicode_xquery_type(t):
        t.assertRaises(
            TypeError,
            lambda : sxq.execute_all(0))
        pass
    
    def test_with_non_unicode_context_type(t):
        t.assertRaises(
            TypeError,
            lambda : sxq.execute_all("/user", 0))
        pass
    
    def test_with_non_callable_resolver_type(t):
        t.assertRaises(
            TypeError,
            lambda : sxq.execute_all("/user", "<_/>", 0))
        pass
    
    def test_with_resolver_cannot_accept_just_one_arg(t):
        t.assertRaises(
            TypeError,
            lambda : sxq.execute_all('doc("foo.xml")/name',
                                     resolver=lambda : "abc"))
        pass
    
    def test_with_resolver_returns_non_unicode_type(t):
        t.assertRaises(
            TypeError,
            lambda : sxq.execute_all('doc("foo.xml")/name',
                                     resolver=lambda uri: 0))
        pass
    
    
    pass


if __name__ == "__main__": unittest.main()
    
