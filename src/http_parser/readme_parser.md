Http Parser 设计架构

为了解析HTTP，我们采用两个类，一个是HttpParser类，主要是进行解析处理，另一个是HttpParserSettings，主要是进行回调函数的设置，刚开始接收到消息是如何处理，接收到请求头部时如何处理，消息接受完毕时如何处理。其实在HTTP解析的时候主要是同步进行处理，因为这时候消息流已经到来，不需要异步处理。

为了能够处理代理，```HttpParser```必须具有分别Request和Response的功能，