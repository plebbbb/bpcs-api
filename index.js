const express = require('express');
const app = express();
const PORT = 8081;
const BPCS = require('./CPP/build/Release/main');


app.use(express.json({limit: '500mb'}))


app.get('/encode', (req, res) => {
    const {data_file} = req.body;
    const datafile = Buffer.from(data_file, "base64");
    const {base_file} = req.body;
    const basefile = Buffer.from(base_file, "base64");
    const {block_height} = req.body;
    const {init_block_factor} = req.body;
    const {final_block_factor} = req.body;
    const {write_block_factor} = req.body;
    const bmp_buffer = BPCS.ENCODE(basefile,datafile, block_height, init_block_factor, final_block_factor, write_block_factor);
    if(typeof bmp_buffer != 'undefined'){
        res.status(200).send({
            output_file : `${bmp_buffer}`,
        });
    }
    else {
        res.status(201).send({

        });
    }
});

app.get('/decode', (req, res) => {
    const {base_file} = req.body;
    const basefile = Buffer.from(base_file, "base64");
    const {block_height} = req.body;
    const {init_block_factor} = req.body;
    const {final_block_factor} = req.body;
    const {write_block_factor} = req.body;
    var data_output = BPCS.DECODE(basefile, block_height, init_block_factor, final_block_factor, write_block_factor);

    if(typeof data_output != 'undefined'){
        res.status(200).send(
            {
                output_file : `${data_output}`
            }
        )
    } else {
        res.status(201).send({

        });
    }
});



app.listen(
    PORT,
    () => console.log(`listening on ${PORT}`)
);