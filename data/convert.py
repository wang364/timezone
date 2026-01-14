def extract_alternatenames_timezone(input_file, output_file):
    """
    从城市数据文件中提取 alternatenames 和 timezone 字段
    如果 alternatenames 为空，则使用 name 字段填充
    
    参数:
        input_file (str): 输入文件路径
        output_file (str): 输出文件路径
    """
    try:
        with open(input_file, 'r', encoding='utf-8') as infile, \
             open(output_file, 'w', encoding='utf-8') as outfile:
            
            for line_num, line in enumerate(infile, 1):
                # 按制表符分割行
                fields = line.strip().split('\t')
                
                # 检查字段数量是否足够
                if len(fields) >= 19:  # 至少需要19个字段（0-18索引）
                    # 提取 alternatenames（索引3）和 timezone（索引17）
                    alternatenames = fields[3]
                    timezone = fields[17]
                    
                    # 如果 alternatenames 为空或只包含空白字符，则使用 name（索引1）
                    if not alternatenames or alternatenames.strip() == '':
                        alternatenames = fields[1]  # 使用 name 字段
                        print(f"第 {line_num} 行: alternatenames 为空，已使用 name 字段: {alternatenames}")
                    
                    # 写入输出文件
                    outfile.write(f"{alternatenames}\t{timezone}\n")
                else:
                    print(f"第 {line_num} 行: 字段不足，跳过")
    
        print(f"\n处理完成！结果已保存到 {output_file}")
        
    except FileNotFoundError:
        print(f"错误：找不到文件 {input_file}")
    except IndexError as e:
        print(f"第 {line_num} 行: 字段索引错误 - {e}")
        print(f"该行有 {len(fields)} 个字段，但至少需要 19 个字段")
    except Exception as e:
        print(f"处理过程中发生错误: {e}")

# 使用示例
if __name__ == "__main__":
    input_file = "cities.txt"  # 输入文件名
    output_file = "cities_alternatenames_timezone.txt"  # 输出文件名
    
    extract_alternatenames_timezone(input_file, output_file)