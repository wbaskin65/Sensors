import thingspeak
import tkinter as tk

channel_id = 2770368  # PUT CHANNEL ID HERE
read_key = "6RP3JV5K0Z6KKAI6"  # PUT YOUR API KEY HERE, (read access only)

channel = thingspeak.Channel(id=channel_id, api_key=read_key, fmt='txt')

class smartTrash:

    def __init__(self):
        self.lid = float(channel.get_field_last(field="1"))
        self.capacity = float(channel.get_field_last(field="2"))
        self.force = float(channel.get_field_last(field="3"))
        self.connectCheck = float(channel.get_last_data_age(field="1"))

        if self.lid == 1:
            self.lid = "Open"
        else:
            self.lid = "Closed"

        if self.connectCheck > 60:
            self.connected = "Error: Not connected"
        else:
            self.connected = "Connected"

        self.tkApp()

    def tkApp(self):
        self.root = tk.Tk()
        self.root.title("Smart Trash Can")

        self.frame1 = tk.LabelFrame(self.root, text="Trash can status:", pady=5, borderwidth=2)
        self.frame1.grid(row=1,column=0)

        self.frame2 = tk.LabelFrame(self.root, text="Data:", pady=5, padx= 5, borderwidth=2)
        self.frame2.grid(row=2,column=0)

        self.connectionStatus = tk.Label(self.frame1, text=f"Connection status: {self.connected}")
        self.connectionStatus.grid(row = 1, column = 0)

        self.lidStatus = tk.Label(self.frame2, text=f"Lid status: {self.lid}")
        self.lidStatus.grid(row=1,column=0)

        self.weight = tk.Label(self.frame2, text=f"Force: {self.force}g")
        self.weight.grid(row=2, column=0)

        self.capacityStatus = tk.Label(self.frame2, text=f"Capacity: {self.capacity}%")
        self.capacityStatus.grid(row=3,column=0)

    def statusCheck(self):
        self.lid = float(channel.get_field_last(field="1"))
        self.capacity = float(channel.get_field_last(field="2"))
        self.force = float(channel.get_field_last(field="3"))
        self.connectCheck = float(channel.get_last_data_age(field="1"))

        if self.lid == 1:
            self.lid = "Open"
        else:
            self.lid = "Closed"

    
    def connectivity(self):
        self.lid = float(channel.get_field_last(field="1"))
        self.capacity = float(channel.get_field_last(field="2"))
        self.force = float(channel.get_field_last(field="3"))
        self.connectCheck = float(channel.get_last_data_age(field="1"))
        
        if self.connectCheck > 60:
            self.connectionStatus.config(text="Disconnected")
        else:   
            self.connectionStatus.config(text="Connected")

        if self.lid == 1:
            self.lid = "Open"
        else:
            self.lid = "Closed"

        self.lidStatus.config(text=f"Lid status: {self.lid}")

        self.weight.config(text=f"Force: {self.force}g")

        self.capacityStatus.config(text=f"Capacity: {self.capacity}%")

        self.root.after(500, self.connectivity)

    def run(self):
        self.connectivity()
        self.root.mainloop()

trashcan = smartTrash()
trashcan.run()