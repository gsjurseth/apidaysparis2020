// grab the things we need
import mongoose from 'mongoose';

const Schema = mongoose.Schema;

// create a schema
const colorsSchema = new Schema({
  color: { type: String, required: true  },
  ip: { type: String, required: true  }
});

// the schema is useless so far
// we need to create a model using it
let colors = mongoose.model('colors', colorsSchema);

// make this available to our users in our Node applications
module.exports = colors;
