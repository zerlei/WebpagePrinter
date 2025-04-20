var ws = require("nodejs-websocket")
const fs = require('fs')
var server = ws.createServer(function (conn) {
    console.log("Connected!")

    let sendArr = [

        // {
        //     Id: "GetPrintInfo",
        //     MsgType: "GetPrintInfo"
        // },
        // {
        //     Id: "AddOnePrintConfig",
        //     MsgType: "AddOnePrintConfig",
        //     Data: {
        //         Name: "Hi" + Math.random(),
        //         PrinterName: "导出为WPS PDF",
        //         TopMargin: 0,
        //         BottomMargin: 1,
        //         LeftMargin: 2,
        //         RightMargin: 2,
        //         Orientation: "横向",
        //         PaperName: "A4"
        //     }
        // },
        // {
        //     Id: "DelOnePrintConfig",
        //     MsgType: "DelOnePrintConfig",
        //     Data: 1//change it
        // },
        // {
        //     Id: "UpdateOnePrintConfig",
        //     MsgType: "UpdateOnePrintConfig",
        //     Data: {
        //         Name: "test213",
        //         PrinterName: "导出为WPS PDF",
        //         TopMargin: 0,
        //         BottomMargin: 1,
        //         LeftMargin: 2,
        //         RightMargin: 2,
        //         Orientation: "横向",
        //         PaperName: "A4"
        //     }
        // },


        // {
        //     Id: "GetPrintConfigs",
        //     MsgType: "GetPrintConfigs"
        // },
        // {
        //     Id: "ToPrint",
        //     MsgType: "ToPrint",
        //     Data: [
        //         {
        //             PageUrl: "https://www.bing.com",
        //             ConfigName: "sdf",
        //             PrintMode: "LoadAchieve"
        //         }
        //     ]
        // },
        // {
        //     Id: "GetPrintedPages",
        //     MsgType: "GetPrintedPages",
        //     Data: {
        //         Size: 20,
        //         Page: 1
        //     }
        // },
        // {
        //     Id: "InsertOrUpdateWebsocketUrl",
        //     MsgType: "InsertOrUpdateWebsocketUrl",
        //     Data: "ws://127.0.0.1:8001"
        // }

        // ,
        {
            Id: "GetWebsocketUrl",
            MsgType: "GetWebsocketUrl"
        }

    ]


    sendArr.forEach(e => {
        conn.sendText(JSON.stringify(e))
    })
    conn.on("text", function (str) {
        let ob = JSON.parse(str)
        fs.writeFile(`./${ob.Id}.json`, str, 'utf8', err => {

        })
    })
    conn.on("close", function (code, reason) {
        console.log("Connection closed")
    })
}).listen(8008)


