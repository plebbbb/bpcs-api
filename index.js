const express = require('express');
const app = express();
const BPCS = require('./CPP/build/Release/main');
const multer = require('multer')
const upload = multer({
    type: Buffer
})


var cors = require('cors')
//app.use(express.json({limit: '500mb'}))

app.use(cors({
    "exposedHeaders": "*"
}))

app.post('/encode',upload.any(), (req, res) => {
    const data_file = req.files[0].buffer;
    const base_file = req.files[1].buffer;
    const {block_height} = req.body;
    const {init_block_factor} = req.body;
    const {final_block_factor} = req.body;
    const {write_block_factor} = req.body;
   // console.log(block_height, init_block_factor, final_block_factor, write_block_factor)
  ///*
    console.log(req.files)
    console.log(req.body)
    console.log(data_file)
    console.log(base_file)
    console.log(init_block_factor)//*/
    const bmp_buffer = BPCS.ENCODE(base_file, data_file, block_height, init_block_factor, final_block_factor, write_block_factor);
    console.log(bmp_buffer)
    if(typeof bmp_buffer != 'undefined'){
        res.writeHead(200, {
            'filename' : Buffer.from(req.files[1].originalname).toString()
        })
    
        res.status(200).end(bmp_buffer);
    }
    else {
        res.status(400).send({});
    }
});

app.post('/decode',upload.any(), (req, res) => {
    const basefile = req.files[0].buffer;
   // const basefile = Buffer.from(base_file, "base64");
    const {block_height} = req.body;
    const {init_block_factor} = req.body;
    const {final_block_factor} = req.body;
    const {write_block_factor} = req.body;
   // console.log(basefile)

    var data_output = BPCS.DECODE(basefile, block_height, init_block_factor, final_block_factor, write_block_factor);

    if(typeof bmp_buffer != 'undefined'){
        res.writeHead(200, {
            'filename' : "output.bin"
        })
    
        res.status(200).end(data_output);
    }
    else {
        res.status(400).send({});
    }
});



const port = process.env.PORT || 8080;

app.listen(
    port,
    () => console.log(`listening on ${port}`)
);