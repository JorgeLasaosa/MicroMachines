/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Multi-layer perceptron tutorial
* A practical application of an MLP in optical character
* recognition.
* written by Lefteris of Real Intelligence Project
*
* you can email for questions/or suggestions
* at :  lefteris@realintelligence.net
* or:   lefkar@msn.com
*
* This is the code for the MLP neural network. At the moment it
* has not been compiled in Linux but it should work fine apart from
* the fact that pdCurses library is not used, for console input to
* make up for windows and <conio.h>. It is in the TODO list.
*
* @author Lefteris
* @date 28/05/2009
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "ANN.h"
#include <fstream>
#include <sstream>

//Multi-layer perceptron constructor
MLP::MLP(int hiddenLayers, int inputN, int hiddenN, int outputN)
	: hiddenL(hiddenLayers), inputN(inputN), hiddenN(hiddenN), outputN(outputN) {

	//let's allocate the memory for the weights
	weights.reserve(inputN*hiddenN+(hiddenN*hiddenN*(hiddenL-1))+hiddenN*outputN);

	//also let's set the size for the neurons vector
	inputNeurons.resize(inputN);
	hiddenNeurons.resize(hiddenN*hiddenL);
	outputNeurons.resize(outputN);


	//randomize weights for inputs to 1st hidden layer
	for(int i = 0; i < inputN*hiddenN; i++) {
		weights.push_back( (( (float)rand() / ((float)(RAND_MAX)+(float)(1)) )) - 0.5 );//[-0.5,0.5]

	}

	//if there are more than 1 hidden layers, randomize their weights
	for(int i=1; i < hiddenL; i++) {
		for(int j = 0; j <  hiddenN*hiddenN; j++) {
			weights.push_back( (( (float)rand() / ((float)(RAND_MAX)+(float)(1)) )) - 0.5   );//[-0.5,0.5]
		}
	}

	//and finally randomize the weights for the output layer
	for(int i = 0; i < hiddenN*outputN; i ++) {
		weights.push_back( (( (float)rand() / ((float)(RAND_MAX)+(float)(1)) )) - 0.5   );//[-0.5,0.5]
	}

}



//destructor
MLP::~MLP() {
	weights.clear();
	inputNeurons.clear();
	hiddenNeurons.clear();
	outputNeurons.clear();
}

void MLP::calculateNetwork() {
	//let's propagate towards the hidden layer
	for(int hidden = 0; hidden < hiddenN; hidden++) {
		hiddenAt(1,hidden) = 0;

		for(int input = 0 ; input < inputN; input ++) {
			hiddenAt(1,hidden) += inputNeurons.at(input)*inputToHidden(input,hidden);
		}

		//and finally pass it through the activation function
		hiddenAt(1,hidden) = sigmoid(hiddenAt(1,hidden));
	}



	//now if we got more than one hidden layers
	for(int i = 2; i <= hiddenL; i ++) {

		//for each one of these extra layers calculate their values
		for(int j = 0; j < hiddenN; j++) { //to
			hiddenAt(i,j) = 0;

			for(int k = 0; k <hiddenN; k++) { //from
				hiddenAt(i,j) += hiddenAt(i-1,k)*hiddenToHidden(i,k,j);
			}

			//and finally pass it through the activation function
			hiddenAt(i,j) = sigmoid(hiddenAt(i,j));
		}
	}

	int i;
	//and now hidden to output
	for(i =0; i< outputN; i ++) {
		outputNeurons.at(i) = 0;


		for(int j = 0; j <hiddenN; j++) {
			outputNeurons.at(i) += hiddenAt(hiddenL,j) * hiddenToOutput(j,i);
		}

		//and finally pass it through the activation function
		outputNeurons.at(i) = sigmoid( outputNeurons.at(i) );
	}
}


//trains the network according to our parameters
bool MLP::trainNetwork(float teachingStep,float lmse,float momentum, const std::string& trainingFile) {

	float mse = 999.0;
	int epochs = 1;
	float error = 0.0;
	//the delta of the output layer
	float* odelta = (float*) malloc(sizeof(float)*outputN);
	//the delta of each hidden layer
	float* hdelta = (float*)  malloc(sizeof(float)*hiddenN*hiddenL);

	//a buffer for the weights
	std::vector<float> tempWeights = weights;
	//used to keep the previous weights before modification, for momentum
	std::vector<float> prWeights = weights;

	// Open training file
	std::string line;

    //For as long as the mse is less than the least mean square error
    while(fabs(mse-lmse) > 0.0001 ) {
        //for each epoch reset the mean square error
        mse = 0.0;

        // Read training file to obtain inputs and goals
        std::vector<float> targetOutput(outputN);
        std::ifstream file(trainingFile);
        while(std::getline(file, line)) {
            std::stringstream ss(line);
            for (int i = 0; i < inputN; i++) {
                ss >> inputNeurons[i];
            }
            for (int i = 0; i < outputN; i++) {
                ss >> targetOutput[i];
            }
            std::cout << "Input: ";
            for(auto &f : inputNeurons) {
                std::cout << f << " ";
            }
            std::cout << std::endl << "Output: ";
            for(auto &f : targetOutput) {
                std::cout << f << " ";
            }
            std::cout << std::endl;

            //then calculate the network
            calculateNetwork();

            //Now we have calculated the network for this iteration
            //let's back-propagate following the back-propagation algorithm

            for(int i = 0; i < outputN; i ++) {
                //let's get the delta of the output layer
                //and the accumulated error
                outputDeltaAt(i) = (targetOutput[i] - outputNeurons[i])*dersigmoid(outputNeurons[i]);
                error += (targetOutput[i] - outputNeurons[i])*(targetOutput[i]-outputNeurons[i]);
            }

            //we start popagating backwards now, to get the error of each neuron
            //in every layer

            //let's get the delta of the last hidden layer first
            for(int i = 0; i < hiddenN; i++) {
                hiddenDeltaAt(hiddenL,i) = 0;//zero the values from the previous iteration
                //add to the delta for each connection with an output neuron
                for(int j = 0; j < outputN; j ++) {
                    hiddenDeltaAt(hiddenL,i) += outputDeltaAt(j) * hiddenToOutput(i,j) ;
                }

                //The derivative here is only because of the
                //delta rule weight adjustment about to follow
                hiddenDeltaAt(hiddenL,i) *= dersigmoid(hiddenAt(hiddenL,i));
            }

            //now for each additional hidden layer, provided they exist
            for(int i = hiddenL-1; i >0; i--) {
                //add to each neuron's hidden delta
                for(int j = 0; j < hiddenN; j ++) { //from

                    hiddenDeltaAt(i,j) = 0;//zero the values from the previous iteration

                    for(int k = 0; k <hiddenN; k++) { //to
                        //the previous hidden layers delta multiplied by the weights
                        //for each neuron
                        hiddenDeltaAt(i,j) +=  hiddenDeltaAt(i+1,k) * hiddenToHidden(i+1,j,k);
                    }

                    //The derivative here is only because of the
                    //delta rule weight adjustment about to follow
                    hiddenDeltaAt(i,j) *= dersigmoid(hiddenAt(i,j));
                }
            }


            //Weights modification
            tempWeights = weights;//keep the previous weights somewhere, we will need them


            //hidden to Input weights
            for(int i = 0; i < inputN; i ++) {
                for(int j = 0; j < hiddenN; j ++) {
                    inputToHidden(i,j) +=   momentum*(inputToHidden(i,j) - _prev_inputToHidden(i,j)) +
                                            teachingStep* hiddenDeltaAt(1,j) * inputNeurons[i];
                }
            }



            //hidden to hidden weights, provided more than 1 layer exists
            for(int i = 2; i <=hiddenL; i++) {

                for(int j = 0; j < hiddenN; j ++) { //from
                    for(int k =0; k < hiddenN; k ++) { //to
                        hiddenToHidden(i,j,k) += momentum*(hiddenToHidden(i,j,k) - _prev_hiddenToHidden(i,j,k)) +
                                                 teachingStep * hiddenDeltaAt(i,k) * hiddenAt(i-1,j);
                    }
                }
            }

            //last hidden layer to output weights
            for(int i = 0; i < outputN; i++) {
                for(int j = 0; j < hiddenN; j ++) {
                    hiddenToOutput(j,i) += momentum*(hiddenToOutput(j,i) - _prev_hiddenToOutput(j,i)) +
                                           teachingStep * outputDeltaAt(i) * hiddenAt(hiddenL,j);
                }
            }

            prWeights = tempWeights;


            //add to the total mse for this epoch
            mse += error/(outputN+1);
            std::cout << mse << std::endl;

            //zero out the error for the next iteration
            error = 0;
        }
        //reset the counter
//            char reply;
//            if((epochs%1000) == 0) {
//                std::cout << "We are at epoch " << epochs <<  ", would you like to continue training? (N for no, any other key to continue)" << std::endl;
//                std::cin >> reply;
//            }

//            if(reply == 'N')
//                break;

//            std::cout << "You will be prompted every 400 epochs if you want to continue training" << std::endl;
//
//            std::cout << "Mean square error for epoch " << epochs << "is: " << mse << std::endl;

        epochs++;
    }
//    //free the goals array
//    //free(goals);
//    //free the deltas
//   // free(odelta);
//    //free(hdelta);

    std::ofstream resultWeights("annWeights.txt");
    if (resultWeights.is_open()) {
        for (float &f : weights) {
            resultWeights << f << std::endl;
        }
    }
    else {
        std::cout << "Failed to create annWeights.txt" << std::endl;
    }

    return true;
}

void MLP::readWeightsFromFile(const std::string& weightsFile)
{
    std::ifstream file(weightsFile);
    if (file.is_open()) {
        int i = 0;
        while(file >> weights[i]) {
            i++;
        }
        for (float &f : weights) {
            std::cout << f << std::endl;
        }
    }
    else {
        std::cout << "Failed to open " << weightsFile << std::endl;
    }
}


void MLP::recallNetwork(const std::vector<float>& input) {
	//first populate the input neurons
	inputNeurons = input;

	//then calculate the network
	calculateNetwork();

	float winner = -1;
	int index = 0;

	std::cout << "Output: ";
	//find the best fitting output
	for(float &f : outputNeurons) {
        std::cout << f << "\t";
	}

//    //output it
//    printf("The neural network thinks that image %d represents a \n\r\n\r \t\t----->| %d |<------\t\t \n\r\n\r",fileNum,index);
//
//    //now let's the exact percentages of what it thnks
//    printf("Analyzing what the network thinks about bitmap %d we see that it thinks it is: \n\r\
//          | 0 with %d%% probability |\n\r\
//          | 1 with %d%% probability |\n\r\
//          | 2 with %d%% probability |\n\r\
//          | 3 with %d%% probability |\n\r\
//          | 4 with %d%% probability |\n\r\
//          | 5 with %d%% probability |\n\r\
//          | 6 with %d%% probability |\n\r\
//          | 7 with %d%% probability |\n\r\
//          | 8 with %d%% probability |\n\r\
//          | 9 with %d%% probability |\n\r\n\r",fileNum,(int)(outputNeurons[0]*100),\
//           (int)(outputNeurons[1]*100),(int)(outputNeurons[2]*100),(int)(outputNeurons[3]*100),\
//           (int)(outputNeurons[4]*100),(int)(outputNeurons[5]*100),(int)(outputNeurons[6]*100),\
//           (int)(outputNeurons[7]*100),(int)(outputNeurons[8]*100),(int)(outputNeurons[9]*100));

}

