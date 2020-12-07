import express from 'express';
import cors from 'cors';
import mongoose from 'mongoose';
import colors from './colors-schema.js';

let mongohost = process.env.mongohost || 'localhost';
let mongoport = process.env.mongoport || 27017;
let mongouser = process.env.MONGO_USERNAME || 'aMagicUser';
let mongopass = process.env.MONGO_PASSWORD || 'aMagicPass';

mongoose.connect(`mongodb://${mongouser}:${mongopass}@${mongohost}:${mongoport}/aMagicDB`, {useNewUrlParser: true});


const app = express();
const port = process.env.PORT || 3000;


app.use(express.json());
app.options('/colors', cors());

app.get('/colors', cors(), (req, res) => {
  colors
  .find()
  .then(doc => {
    //console.log(doc);
    let clrs = {
      "red": 0,
      "blue": 0,
      "green": 0
    };
    doc.forEach( x => {
      clrs[ x.color ] = clrs[ x.color ] + 1;
    });

    let percentages = {
      "red": (clrs.red/doc.length).toFixed(3).toString(),
      "blue": (clrs.blue/doc.length).toFixed(3).toString(),
      "green": (clrs.green/doc.length).toFixed(3).toString()
    }
    return res.json(percentages);
  })
  .catch(err => {
    console.error(err);
    return res.status(500).send(err);
  });
});

app.post('/colors', cors(), (req, res) => {
  console.log("this is the body: %j", req.body)
  colors.insertMany( req.body )
   .then(doc => {
     //console.log(doc)
     return res.json(doc);
   })
   .catch(err => {
     console.error(err)
     return res.status(500).send(err);
   });
});

app.delete('/colors', cors(), (req, res) => {
  console.log("Looks like we're wiping out our results")
  colors.remove()
   .then(doc => {
     //console.log(doc)
     return res.json({ "msg": "we are clean" });
   })
   .catch(err => {
     console.error(err)
     return res.status(500).send(err);
   });
});

app.listen(port, () =>
  console.log(`Example app listening on port ${port}!`),
);
