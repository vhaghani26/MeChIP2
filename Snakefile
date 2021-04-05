# List of SRA numbers for samples of interest
# Modify SAMPLES for personal use
SAMPLES=["SRR5785190"]

rule all:
    input: 
        expand("{sample}.bw", sample=SAMPLES)

# This rule currently works in dry run, but I need the code to actually work - figure out for loop stuff
rule download_data:
    message: "Downloading raw data files"
    output: protected("{sample}.sra")
    params: 
	SRA_code = "{sample}", sample=SAMPLES)"
    shell: "prefetch {params.SRA_code}"

rule split_paired_reads:
    input: "{sample}.sra"
    output:
    shell: ### "fastq-dump {sample}.sra --split-files --outdir ../files"

rule gzip_data:
    input: 
    output:
    shell: #### "gzip files/SRR*"

rule download_genome:
    output: "mm39.chromFa.tar.gz"
    shell: "wget https://hgdownload.soe.ucsc.edu/goldenPath/mm39/bigZips/mm39.chromFa.tar.gz"
    
rule decompress_genome:
    input: "mm39.chromFa.tar.gz"
    output: #######
    shell: "tar zvfx {input}"

rule concatenate_chromosomes:
    input: ######
    output: protected("mm39.fa")
    shell: "cat *.fa > {output}" #######

rule delete_chromosome_files:
    input:
    output:
    shell: "rm chr*.fa   " ###### 
    
rule set_alignment_reference:
    input: "mm39.fa"
    output:
    shell: "bwa index -p mm39 -a bwtsw {input}" 

rule align_reads:
    input:
        r1 = "{sample}_1.fastq.gz",
        r2 = "{sample}_2.fastq.gz"
    output: "{sample}.sam"
    shell: "bwa mem mm39 {sample}_1.fastq.gz {sample}_2.fastq.gz > {sample}.sam"
    
rule sam_to_bam:
    input: "{sample}.sam"
    output: "{sample}.bam"
    shell: "samtools view -b {input} > {output}"

rule sam_fixmate:
    input: "{sample}.bam"
    output: "{sample}.namesorted.fixmate.bam"
    shell: "samtools fixmate -rcm -O bam {input} {output}"

rule sam_sort:
    input: "{sample}.namesorted.fixmate.bam"
    output: "{sample}.coorsorted.fixmate.bam "
    shell: "samtools sort -o {output} {input}"

rule sam_markdup:
    input: "{sample}.coorsorted.fixmate.bam"
    output: "{sample}.coorsorted.dedup.bam"
    shell: "samtools markdup -r --mode s {input} {output}"

rule sam_index:
    input: "{sample}.coorsorted.dedup.bam"
    output: "{sample}.indexed.dedup.bam"
    shell: "samtools index {input}"

rule bam_to_bigwig:
    input: "{sample}.indexed.dedup.bam"
    output: "{sample}.bw"
    shell: "bamCoverage -b {input} -o {output}"