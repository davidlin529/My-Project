# CCBB Notebook - Week 28 (1/22-1/25)
#### Name: John David Lin

## Date: 1/22/2019 (Tue.)

### 1. Debrief with Tom

Kriti talked to Tom about Annotatr and I talked to him about NetSig. 

### 2. Integration Methods Plan

- Tom wants 3 things of integration
  1. iClusterPlus
  2. WCGNA - weighted gene correlation network analysis 
  3. CCA, CIA or Dim. Red. - ask Tom's recommendation 
- Make a table of the integration methods and their input types from RNA-seq and ATAC-seq. 
- Integration -> gene list -> pathways -> compare the integration methods
  - Comparisons of the integration:
    - validation with data
    - interpretation
- Exploration and interpretation
- CCBB toolbox or a showcase of what software we have explored so far / summary 

### 3. NetSig

#### NetSig input

P values or q values?
- P values are the probability that the significance is due to randomness. 
- q values are the FDR, adjusted for multiple testing
- Adjusted P values are also adjusted for multiple testing
- Better to use adjusted P values or q values over P values

Since RNA-seq, ATAC-seq have some sort of adjusted P values or P values, we can use them for NetSig input. Not just MutSig's q values as NetSig's input. 

Questions: 
- Does NetSig take in all genes or just the significant genes for input?
- Does NetSig take the directionality of the change into account? (Up or down)
  - Separate parallel analysis
  - But can get very large and complicated 

#### NetSig output

NetSig output is a set of P values (NetSig scores)

Questions: 
- How to subset the output that are significant?
- Use the scores for the analysis?
- What to do with the output? 

#### Integration

- Usually increases in open regions correspond to increase in expression level
- How to integration RNA-seq and ATAC-seq because NetSig doesn't do integration?
   - The integration parts have to be before or after NetSig
- Use NetSig for RNA-seq and ATAC-seq parallelly and use the 2 set of NetSig scores as inputs of integration methods?
- What added values does NetSig have in the big picture of integration?

## Date: 1/24/2019 (Thu.)

### 1. iClusterPlus

- Worked through the tutorial. 
- 4. Model Tuning took a long time to run (5 days to complete). 
- Worked on getting iClusterPlus to run on our real data, but stuck at formatting the dataframe. 

### 2. Discussion with Tom

1. Dataset comparison: quantification vs quantificaiton
  -  Quant. vs. quant. isn't just getting DE genes. DE genes are resulted from a single experiment. If we compare 2 different experiments, 2 different datasets, or miRNA-seq vs RNA-seq data, then the results wouldn't just be DE genes. So we need to develop / use a different technique for such comparisons. 
2. WCGNA: integrating before it to get a set of eigengenes or after it to combine the 2 sets of eigengenes.
3. CCA/CIA: read the paper he sent us and one he sent us before. I have to implement it myself. It's best for Kriti to work on WCGNA and me to work on implementing CCA/CIA, because she's better in R and I'm better in programming. 

## Date: 1/25/2019 (Fri.)

### 1. iClusterPlus

- Running iClusterPlus on Shalapour data. 
- Formatted the Shalapour data to match the format of the test data. 
- Attempted to make iClusterPlus usable in my EC2 to take advantage of the computing node, but installing it threw some errors and all attempts of working aroud it failed. 

